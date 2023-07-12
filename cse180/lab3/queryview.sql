
SELECT e.highwayNum, h.length, COUNT(*) AS openMisdirectedExitCount
FROM MisdirectedExitView m, Exits e, Highways h 
WHERE h.highwayNum = e.highwayNum
AND m.highwayNum = h.highwayNum
AND e.exitNum = m.exitNum
AND e.isExitOpen = TRUE
GROUP BY e.highwayNum, h.length;
/* 
highwaynum | length | openmisdirectedexitcount 
------------+--------+--------------------------
          1 |  150.3 |                        1
         13 |   45.5 |                        2
         17 |  102.6 |                        3
        280 |  200.9 |                        1
(4 rows)
*/

DELETE FROM Exits
WHERE highwayNum = 17 AND exitNum = 34;

DELETE FROM Exits 
WHERE highwayNum = 280 AND exitNum= 95;

SELECT e.highwayNum, h.length, COUNT(*) AS openMisdirectedExitCount
FROM MisdirectedExitView m, Exits e, Highways h 
WHERE h.highwayNum = e.highwayNum
AND m.highwayNum = h.highwayNum
AND e.exitNum = m.exitNum
AND e.isExitOpen = TRUE
GROUP BY e.highwayNum, h.length;
/*
  highwaynum | length | openmisdirectedexitcount 
------------+--------+--------------------------
          1 |  150.3 |                        1
         13 |   45.5 |                        2
         17 |  102.6 |                        2
(3 rows)
*/