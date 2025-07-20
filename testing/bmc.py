def sol_activate_and_quit(bmc_ip: str | None=None,
                           ipmi_user: str | None=None,
                           ipmi_pass: str | None=None,
                           channel: str = 'host',
                           coap: bool = True,
                           timeout: int = 3,
                           max_retry: int = 1,
                           backoff: bool = True,
                           match_str: str = "Starting SOL session"
) -> Tuple[bool, str]:
    if bmc_ip is None:
        bmc_ip = args.bmcIp
    if ipmi_user is None:
        ipmi_user = args.bmcUser
    if ipmi_pass is None:
        ipmi_pass = args.bmcPwd

    """
    透過 pexpect 在 pty 中執行 nitro-bmc sol activate，並自動送 ~. 結束。
    參數
    ----
    bmc_ip     : str    # BMC IP
    ipmi_user  : str    # IPMI 登入帳號
    ipmi_pass  : str    # IPMI 登入密碼
    channel    : str    # SOL channel 名稱 (預設 host)
    coap       : bool   # 是否加 --coap
    timeout    : int    # 等待 SOL 啟動／截取畫面最長秒數 (若需擴大可調)
    max_retry  : int    # 若 activate 失敗，最多重試次數
    backoff    : bool   # 重試時是否使用 1→2→4… 秒退避

    回傳
    ----
    (success: bool, session_output: str)
      - success 為 True: 代表 activate + 結束都正常
      - session_output: SOL 期間讀到的所有輸出 (不含 ~. 本身)
    """

    base_cmd = [
        'nitro-bmc',
        '-i',  args.bmcIp,
        'sol', 'activate',
        '--channel-name', channel
    ]
    if coap:
        base_cmd.append('--coap')

    last_error = ""
    session_output = ""

    for attempt in range(1, max_retry + 1):
        try:
            # 用 pexpect.spawn 建立一個 pty 來執行 sol activate
            cmd_str = " ".join(base_cmd)
            print(f"[attempt {attempt}] {cmd_str}")
            child = pexpect.spawn(cmd_str, encoding='utf-8', timeout=timeout)
            child.logfile = sys.stdout

            child.expect("Starting SOL session", timeout=timeout)
            
            # 讀取途中輸出
            session_output += child.before
            
            child.sendcontrol("m")
            child.sendcontrol("m")
            child.expect(match_str, timeout=timeout)
            


            # 3. 自動送出 ~. 結束 SOL
            child.send("~.")
            child.sendcontrol("m")

            # 4. 等到 SOL 會話斷線
            child.expect(pexpect.EOF, timeout=timeout)
            session_output += child.before
            child.close()
            return True, session_output

        except (pexpect.TIMEOUT, pexpect.EOF, RuntimeError) as e:
            last_error = f"[attempt {attempt}] {e}"
        except Exception as e:
            last_error = f"[attempt {attempt}] unexpected: {e}"
        finally:
            if child is not None and child.isalive():
                child.close(force=True)

        # 如果還要重試，先等一下（backoff：1,2,4 秒）
        if attempt < max_retry and backoff:
            sleep_sec = min(2 ** (attempt - 1), 8)
            time.sleep(sleep_sec)
    
    # 全部都嘗試失敗
    cmd = ['sol', 'deactivate', '--channel-name', channel, '--coap']
    out = nitro(cmd)
    return False, last_error