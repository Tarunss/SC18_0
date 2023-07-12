SELECT DISTINCT e1.exitNum as firstExitNum,e2.exitNum as secondExitNum, (e2.mileMarker-e1.mileMarker) as exitDistance
FROM Exits e1, Exits e2
WHERE e1.highwayNum = e2.highwayNum
AND e1.mileMarker < e2.mileMarker
AND (e2.mileMarker - e1.mileMarker) <= 5;