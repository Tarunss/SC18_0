SELECT h.highwayNum, length
FROM Highways h
WHERE length > 100
AND highwayNum NOT IN
(SELECT highwayNum FROM Cameras WHERE isCameraWorking = TRUE)
ORDER by length DESC;