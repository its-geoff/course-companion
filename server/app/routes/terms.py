from datetime import date

import pymysql
from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from pymysql.connections import Connection

from app.database import get_db

router = APIRouter()


class TermCreate(BaseModel):
    id: str
    title: str
    start_date: date
    end_date: date
    active: bool = True


class TermResponse(TermCreate):
    pass


@router.get("/", response_model=list[TermResponse])
def get_terms(db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, title, start_date, end_date, active FROM terms"
        )
        return cursor.fetchall()


@router.get("/{term_id}", response_model=TermResponse)
def get_term(term_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, title, start_date, end_date, active FROM terms WHERE "
            "id = %s",
            (term_id,),
        )
        term = cursor.fetchone()
    if not term:
        raise HTTPException(status_code=404, detail="Term not found")
    return term


@router.post("/", response_model=TermResponse, status_code=201)
def create_term(term: TermCreate, db: Connection = Depends(get_db)):
    try:
        with db.cursor() as cursor:
            cursor.execute(
                "INSERT INTO terms (id, title, start_date, end_date, active) "
                "VALUES (%s, %s, %s, %s, %s)",
                (
                    term.id,
                    term.title,
                    term.start_date,
                    term.end_date,
                    term.active,
                ),
            )
        db.commit()
    except pymysql.IntegrityError:
        raise HTTPException(
            status_code=409, detail="A term with this title already exists"
        )
    return term


@router.put("/{term_id}", response_model=TermResponse)
def update_term(
    term_id: str, term: TermCreate, db: Connection = Depends(get_db)
):
    try:
        with db.cursor() as cursor:
            cursor.execute(
                "UPDATE terms SET title = %s, start_date = %s, end_date = %s, "
                "active = %s WHERE id = %s",
                (
                    term.title,
                    term.start_date,
                    term.end_date,
                    term.active,
                    term_id,
                ),
            )
        db.commit()
    except pymysql.IntegrityError:
        raise HTTPException(
            status_code=409, detail="A term with this title already exists"
        )
    if cursor.rowcount == 0:
        raise HTTPException(status_code=404, detail="Term not found")
    return term


@router.delete("/{term_id}", status_code=204)
def delete_term(term_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM terms WHERE id = %s", (term_id,))
    db.commit()
    if cursor.rowcount == 0:
        raise HTTPException(status_code=404, detail="Term not found")
