import mysql.connector

mydb = mysql.connector.connect(
    host="localhost",
    user="root",
    passwd="12345678",
    auth_plugin='mysql_native_password',
    database="mydb"
)
mycursor = mydb.cursor()
sql = "INSERT INTO `mask`(`id`, `adult`, `child`, `time`) VALUES (%s, %s, %s, %s)"
val = (1,1,1,"2020/03/04 22:00:40")
mycursor.execute(sql, val)
mydb.commit()

print(mycursor.rowcount, "record inserted.")