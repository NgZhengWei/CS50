SELECT AVG(rating) AS 'Average Rating' FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);
/* OR
SELECT AVG(ratings.rating) AS 'Average Rating' FROM ratings JOIN movies ON ratings.movie_id = movies.id WHERE movies.year = 2012;
*/