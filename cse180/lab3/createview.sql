CREATE VIEW MisdirectedExitView AS
SELECT e.highwayNum, e.exitNum,e.mileMarker, COUNT(*) AS misdirectedProverCount 
FROM Exits e, Exits e2
WHERE e.highwayNum = e2.highwayNum
AND e.exitNum < e2.exitNum  
AND e.mileMarker > e2.mileMarker
GROUP BY e.highwayNum,e.exitNum,e.mileMarker
HAVING COUNT(*) >= 2;

