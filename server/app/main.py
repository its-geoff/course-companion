from fastapi import FastAPI

from app.routes import assignments, courses, terms

app = FastAPI(title="Course Companion API")

app.include_router(terms.router, prefix="/terms", tags=["terms"])
app.include_router(courses.router, prefix="/courses", tags=["courses"])
app.include_router(
    assignments.router, prefix="/assignments", tags=["assignments"]
)
