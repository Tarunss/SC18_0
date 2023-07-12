-- CSE 180 Winter 2023 Lab2 create which you must modify as described in Lab2 pdf

-- The following two lines are not needed i but they're useful.
DROP SCHEMA Lab2 CASCADE;
CREATE SCHEMA Lab2;


CREATE TABLE Highways (
    highwayNum INT,
    length NUMERIC(4,1) NOT NULL,
    speedLimit INT,
    PRIMARY KEY (highwayNum)
);

CREATE TABLE Exits (
    highwayNum INT REFERENCES Highways,
    exitNum INT,
    description VARCHAR(60),
    mileMarker NUMERIC(4,1) NOT NULL,
    exitCity VARCHAR(20),
    exitState CHAR(2),
    isExitOpen BOOL,
    PRIMARY KEY (highwayNum, exitNum),
    UNIQUE (highwayNum,mileMarker)
);

CREATE TABLE Interchanges (
    highwayNum1 INT,
    exitNum1 INT,
    highwayNum2 INT,
    exitNum2 INT,
    FOREIGN KEY (highwayNum1, exitNum1) REFERENCES Exits(highwayNum, exitNum),
    FOREIGN KEY (highwayNum2, exitNum2) REFERENCES Exits(highwayNum, exitNum),
    PRIMARY KEY (highwayNum1, exitNum1, highwayNum2, exitNum2)
);

CREATE TABLE Cameras (
    cameraID INT,
    highwayNum INT REFERENCES Highways(highwayNum),
    mileMarker NUMERIC(4,1) NOT NULL,
    isCameraWorking BOOL,
    PRIMARY KEY (cameraID)
);

CREATE TABLE Owners (
    ownerState CHAR(2),
    ownerLicenseID CHAR(8),
    name VARCHAR(60),
    address VARCHAR(60),
    startDate DATE,
    expirationDate DATE,
    PRIMARY KEY (ownerState, ownerLicenseID),
    UNIQUE (name,address)
);

CREATE TABLE Vehicles (
    vehicleState CHAR(2),
    vehicleLicensePlate CHAR(7),
    ownerState CHAR(2),
    ownerLicenseID CHAR(8),
    year INT,
    color CHAR(2),
    FOREIGN KEY (ownerState, ownerLicenseID) REFERENCES Owners,
    PRIMARY KEY (vehicleState, vehicleLicensePlate)
);

CREATE TABLE Photos (
    cameraID INT REFERENCES Cameras,
    vehicleState CHAR(2),
    vehicleLicensePlate Char(7),
    photoTimestamp TIMESTAMP,
    FOREIGN KEY (vehicleState, vehicleLicensePlate) REFERENCES Vehicles,
    PRIMARY KEY (cameraID, photoTimestamp),
    UNIQUE (vehicleState,vehicleLicensePlate,photoTimestamp)
);