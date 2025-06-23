# db.py
import mysql.connector

conn = mysql.connector.connect(
    host='localhost',
    user='root',
    password='12345',
    database='smarthome'
)

db = conn.cursor(dictionary=True)
