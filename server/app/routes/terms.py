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


def handle_integrity_error(error: pymysql.IntegrityError):
    error_code = error.args[0]
    error_message = str(error)

    if error_code == 1062:
        if "title" in error_message:
            raise HTTPException(
                status_code=409,
                detail="A term with this title already exists",
            )

        if "PRIMARY" in error_message or "id" in error_message:
            raise HTTPException(
                status_code=409,
                detail="A term with this ID already exists",
            )

    raise HTTPException(
        status_code=400,
        detail="Database constraint violation",
    )


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

    except pymysql.IntegrityError as error:
        handle_integrity_error(error)

    return term


@router.put("/{term_id}", response_model=TermResponse)
def update_term(
    term_id: str, term: TermCreate, db: Connection = Depends(get_db)
):
    rowcount = 0

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

            rowcount = cursor.rowcount

        db.commit()

    except pymysql.IntegrityError as error:
        handle_integrity_error(error)

    if rowcount == 0:
        raise HTTPException(status_code=404, detail="Term not found")

    return term


@router.delete("/{term_id}", status_code=204)
def delete_term(term_id: str, db: Connection = Depends(get_db)):
    rowcount = 0

    with db.cursor() as cursor:
        cursor.execute("DELETE FROM terms WHERE id = %s", (term_id,))
        rowcount = cursor.rowcount

    db.commit()

    if rowcount == 0:
        raise HTTPException(status_code=404, detail="Term not found")
