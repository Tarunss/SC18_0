SELECT highwayNum1 as h1,exitNum1 as e1, highwayNum2 as h2, exitNum2 as e2
FROM Interchanges i JOIN Exits e ON i.exitNum1 = e.exitNum AND i.highwayNum1 = e.highwayNum
JOIN Exits f ON i.exitNum2 = f.exitNum AND i.highwayNum2 = f.highwayNum
WHERE e.description = f.description
AND f.isExitOpen IS TRUE
AND e.isExitOpen IS TRUE
AND (e.exitCity <> f.exitCity OR e.exitState <> f.exitState);
