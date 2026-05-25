import os

import pymysql
import pymysql.cursors
from dotenv import load_dotenv

load_dotenv()


def get_db():
    conn = pymysql.connect(
        host=os.getenv("DB_HOST") or "localhost",
        port=int(os.getenv("DB_PORT") or 3306),
        user=os.getenv("DB_USER")
        or (_ for _ in ()).throw(ValueError("DB_USER not set")),
        password=os.getenv("DB_PASSWORD")
        or (_ for _ in ()).throw(ValueError("DB_PASSWORD not set")),
        database=os.getenv("DB_NAME")
        or (_ for _ in ()).throw(ValueError("DB_NAME not set")),
        cursorclass=pymysql.cursors.DictCursor,
    )
    try:
        yield conn
    finally:
        conn.close()
