import pandas as pd
from sqlalchemy import create_engine

# MySQL的使用者：root, 密碼:147369, 埠：3306,資料庫：mydb
engine = create_engine('mysql+pymysql://root:12345678@localhost:3306/db_mask')
# 將pandas dataframe 的 data 新增到 mysql mask資料表
data.to_sql('mask', engine,if_exists='append')

