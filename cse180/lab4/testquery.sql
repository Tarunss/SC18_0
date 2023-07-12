SELECT c.cameraID,c.highwayNum,c.mileMarker, COUNT(Photos)
FROM Cameras c, Photos p
GROUP BY c.cameraID;