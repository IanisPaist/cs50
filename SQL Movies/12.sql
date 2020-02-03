SELECT title FROM movies

WHERE movies.id IN (
SELECT movies.id FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE name = "Johnny Depp")

AND movies.id IN (
SELECT movies.id FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE name = "Helena Bonham Carter")