SELECT name from people
JOIN stars ON people.id = stars.person_id
JOIN movies ON movies.id = stars.movie_id
WHERE year = 2004
GROUP BY name
ORDER BY birth
