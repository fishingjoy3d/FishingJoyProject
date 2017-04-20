/*
Navicat MySQL Data Transfer

Source Server         : root
Source Server Version : 50626
Source Host           : localhost:3306
Source Database       : fishlog

Target Server Type    : MYSQL
Target Server Version : 50626
File Encoding         : 65001

Date: 2016-06-01 19:42:03
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for fishaddrechargelog
-- ----------------------------
DROP TABLE IF EXISTS `fishaddrechargelog`;
CREATE TABLE `fishaddrechargelog` (
  `UserID` int(10) unsigned NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrcey` int(10) unsigned NOT NULL,
  `AddOrderUserName` varchar(16) NOT NULL,
  `AddOrderUserIP` varchar(16) NOT NULL,
  `AddOrderLogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishannouncementinfolog
-- ----------------------------
DROP TABLE IF EXISTS `fishannouncementinfolog`;
CREATE TABLE `fishannouncementinfolog` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `NickName` varchar(11) NOT NULL,
  `ShopID` tinyint(4) unsigned NOT NULL,
  `ShopOnlyID` tinyint(4) unsigned NOT NULL,
  `LogTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo`;
CREATE TABLE `fishcartableinfo` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_5_25
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_5_25`;
CREATE TABLE `fishcartableinfo_2016_5_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=175 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_5_26`;
CREATE TABLE `fishcartableinfo_2016_5_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1680 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_5_27`;
CREATE TABLE `fishcartableinfo_2016_5_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1681 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_5_28`;
CREATE TABLE `fishcartableinfo_2016_5_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1682 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_5_29`;
CREATE TABLE `fishcartableinfo_2016_5_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1682 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_5_30`;
CREATE TABLE `fishcartableinfo_2016_5_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1678 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_5_31`;
CREATE TABLE `fishcartableinfo_2016_5_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1634 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishcartableinfo_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishcartableinfo_2016_6_1`;
CREATE TABLE `fishcartableinfo_2016_6_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` tinyint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) unsigned NOT NULL,
  `SystemGlobel` bigint(20) unsigned NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1380 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo`;
CREATE TABLE `fishdialtableinfo` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo_2016_5_26`;
CREATE TABLE `fishdialtableinfo_2016_5_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=550 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo_2016_5_27`;
CREATE TABLE `fishdialtableinfo_2016_5_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1305 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo_2016_5_28`;
CREATE TABLE `fishdialtableinfo_2016_5_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1305 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo_2016_5_29`;
CREATE TABLE `fishdialtableinfo_2016_5_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1306 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo_2016_5_30`;
CREATE TABLE `fishdialtableinfo_2016_5_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1302 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo_2016_5_31`;
CREATE TABLE `fishdialtableinfo_2016_5_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1268 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishdialtableinfo_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishdialtableinfo_2016_6_1`;
CREATE TABLE `fishdialtableinfo_2016_6_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandUserID` int(10) unsigned NOT NULL,
  `AreaData` varchar(256) NOT NULL,
  `AreaGlobel` varchar(1024) NOT NULL,
  `ResultIndex` smallint(3) unsigned NOT NULL,
  `BrandGlobel` bigint(20) NOT NULL,
  `SystemGlobel` bigint(20) NOT NULL,
  `RoleSum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1071 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishentityitemfinishlog
-- ----------------------------
DROP TABLE IF EXISTS `fishentityitemfinishlog`;
CREATE TABLE `fishentityitemfinishlog` (
  `ID` int(10) unsigned NOT NULL,
  `OrderID` varchar(64) NOT NULL,
  `HandleTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishentityitemlog
-- ----------------------------
DROP TABLE IF EXISTS `fishentityitemlog`;
CREATE TABLE `fishentityitemlog` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `OrderStates` varchar(256) NOT NULL DEFAULT '',
  `UserID` int(11) unsigned NOT NULL,
  `ItemID` int(11) unsigned NOT NULL,
  `ItemSum` smallint(6) unsigned NOT NULL,
  `Address` varchar(256) NOT NULL DEFAULT '',
  `Phone` varchar(20) NOT NULL DEFAULT '',
  `IDEntity` varchar(256) NOT NULL DEFAULT '',
  `Name` varchar(256) NOT NULL DEFAULT '',
  `OrderNumber` varchar(256) NOT NULL DEFAULT '',
  `MedalNum` int(10) unsigned NOT NULL DEFAULT '0',
  `NowMedalNum` int(10) unsigned NOT NULL DEFAULT '0',
  `HandleIP` varchar(16) NOT NULL DEFAULT '',
  `ShopLogTime` datetime NOT NULL DEFAULT '1971-01-01 00:00:00',
  `HandleTime` datetime NOT NULL DEFAULT '1971-01-01 00:00:00',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishentityitemlog_2015_12_25
-- ----------------------------
DROP TABLE IF EXISTS `fishentityitemlog_2015_12_25`;
CREATE TABLE `fishentityitemlog_2015_12_25` (
  `UserID` int(11) unsigned NOT NULL,
  `ItemID` int(11) unsigned NOT NULL,
  `ItemSum` smallint(6) unsigned NOT NULL,
  `Address` varchar(129) NOT NULL,
  `Phone` varchar(33) NOT NULL,
  `Name` varchar(11) NOT NULL,
  `ShopTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog`;
CREATE TABLE `fishexchangelog` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_2_24
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_2_24`;
CREATE TABLE `fishexchangelog_2016_2_24` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_2_25
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_2_25`;
CREATE TABLE `fishexchangelog_2016_2_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_2_26
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_2_26`;
CREATE TABLE `fishexchangelog_2016_2_26` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_2_27
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_2_27`;
CREATE TABLE `fishexchangelog_2016_2_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_2_29
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_2_29`;
CREATE TABLE `fishexchangelog_2016_2_29` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_11
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_11`;
CREATE TABLE `fishexchangelog_2016_3_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_15
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_15`;
CREATE TABLE `fishexchangelog_2016_3_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_16
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_16`;
CREATE TABLE `fishexchangelog_2016_3_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_17
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_17`;
CREATE TABLE `fishexchangelog_2016_3_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_2
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_2`;
CREATE TABLE `fishexchangelog_2016_3_2` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_21
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_21`;
CREATE TABLE `fishexchangelog_2016_3_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_25
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_25`;
CREATE TABLE `fishexchangelog_2016_3_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_27
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_27`;
CREATE TABLE `fishexchangelog_2016_3_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_3
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_3`;
CREATE TABLE `fishexchangelog_2016_3_3` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_31
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_31`;
CREATE TABLE `fishexchangelog_2016_3_31` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_4
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_4`;
CREATE TABLE `fishexchangelog_2016_3_4` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_5
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_5`;
CREATE TABLE `fishexchangelog_2016_3_5` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_6
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_6`;
CREATE TABLE `fishexchangelog_2016_3_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_7
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_7`;
CREATE TABLE `fishexchangelog_2016_3_7` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_3_8
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_3_8`;
CREATE TABLE `fishexchangelog_2016_3_8` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_1
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_1`;
CREATE TABLE `fishexchangelog_2016_4_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_11
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_11`;
CREATE TABLE `fishexchangelog_2016_4_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_12
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_12`;
CREATE TABLE `fishexchangelog_2016_4_12` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_13
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_13`;
CREATE TABLE `fishexchangelog_2016_4_13` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_15
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_15`;
CREATE TABLE `fishexchangelog_2016_4_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_16
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_16`;
CREATE TABLE `fishexchangelog_2016_4_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_17
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_17`;
CREATE TABLE `fishexchangelog_2016_4_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_18
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_18`;
CREATE TABLE `fishexchangelog_2016_4_18` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_19
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_19`;
CREATE TABLE `fishexchangelog_2016_4_19` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_2
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_2`;
CREATE TABLE `fishexchangelog_2016_4_2` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_20
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_20`;
CREATE TABLE `fishexchangelog_2016_4_20` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_21
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_21`;
CREATE TABLE `fishexchangelog_2016_4_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_22
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_22`;
CREATE TABLE `fishexchangelog_2016_4_22` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_23
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_23`;
CREATE TABLE `fishexchangelog_2016_4_23` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_24
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_24`;
CREATE TABLE `fishexchangelog_2016_4_24` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_25
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_25`;
CREATE TABLE `fishexchangelog_2016_4_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_26
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_26`;
CREATE TABLE `fishexchangelog_2016_4_26` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_27
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_27`;
CREATE TABLE `fishexchangelog_2016_4_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_29
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_29`;
CREATE TABLE `fishexchangelog_2016_4_29` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_30
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_30`;
CREATE TABLE `fishexchangelog_2016_4_30` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_4
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_4`;
CREATE TABLE `fishexchangelog_2016_4_4` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_4_6
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_4_6`;
CREATE TABLE `fishexchangelog_2016_4_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_1
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_1`;
CREATE TABLE `fishexchangelog_2016_5_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_10
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_10`;
CREATE TABLE `fishexchangelog_2016_5_10` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_11
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_11`;
CREATE TABLE `fishexchangelog_2016_5_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_12
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_12`;
CREATE TABLE `fishexchangelog_2016_5_12` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_13
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_13`;
CREATE TABLE `fishexchangelog_2016_5_13` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_14
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_14`;
CREATE TABLE `fishexchangelog_2016_5_14` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_15
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_15`;
CREATE TABLE `fishexchangelog_2016_5_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_16
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_16`;
CREATE TABLE `fishexchangelog_2016_5_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_17
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_17`;
CREATE TABLE `fishexchangelog_2016_5_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_18
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_18`;
CREATE TABLE `fishexchangelog_2016_5_18` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_19
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_19`;
CREATE TABLE `fishexchangelog_2016_5_19` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_2
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_2`;
CREATE TABLE `fishexchangelog_2016_5_2` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_20
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_20`;
CREATE TABLE `fishexchangelog_2016_5_20` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_21
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_21`;
CREATE TABLE `fishexchangelog_2016_5_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_22
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_22`;
CREATE TABLE `fishexchangelog_2016_5_22` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_23
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_23`;
CREATE TABLE `fishexchangelog_2016_5_23` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_24
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_24`;
CREATE TABLE `fishexchangelog_2016_5_24` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_25
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_25`;
CREATE TABLE `fishexchangelog_2016_5_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_26`;
CREATE TABLE `fishexchangelog_2016_5_26` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_27`;
CREATE TABLE `fishexchangelog_2016_5_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_28`;
CREATE TABLE `fishexchangelog_2016_5_28` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_29`;
CREATE TABLE `fishexchangelog_2016_5_29` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_3
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_3`;
CREATE TABLE `fishexchangelog_2016_5_3` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_30`;
CREATE TABLE `fishexchangelog_2016_5_30` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_31`;
CREATE TABLE `fishexchangelog_2016_5_31` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_4
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_4`;
CREATE TABLE `fishexchangelog_2016_5_4` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_5
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_5`;
CREATE TABLE `fishexchangelog_2016_5_5` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_6
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_6`;
CREATE TABLE `fishexchangelog_2016_5_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_7
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_7`;
CREATE TABLE `fishexchangelog_2016_5_7` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_5_9
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_5_9`;
CREATE TABLE `fishexchangelog_2016_5_9` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishexchangelog_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishexchangelog_2016_6_1`;
CREATE TABLE `fishexchangelog_2016_6_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` tinyint(3) unsigned NOT NULL,
  `ExChange` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog
-- ----------------------------
DROP TABLE IF EXISTS `fishlog`;
CREATE TABLE `fishlog` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo`;
CREATE TABLE `fishlogmonthinfo` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_2_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_2_29`;
CREATE TABLE `fishlogmonthinfo_2016_2_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=89 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_10`;
CREATE TABLE `fishlogmonthinfo_2016_3_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7643 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_11`;
CREATE TABLE `fishlogmonthinfo_2016_3_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7860 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_12`;
CREATE TABLE `fishlogmonthinfo_2016_3_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7826 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_13
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_13`;
CREATE TABLE `fishlogmonthinfo_2016_3_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7803 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_14`;
CREATE TABLE `fishlogmonthinfo_2016_3_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7822 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_15`;
CREATE TABLE `fishlogmonthinfo_2016_3_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7864 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_16`;
CREATE TABLE `fishlogmonthinfo_2016_3_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=218 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_17`;
CREATE TABLE `fishlogmonthinfo_2016_3_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3961 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_18`;
CREATE TABLE `fishlogmonthinfo_2016_3_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7505 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_19`;
CREATE TABLE `fishlogmonthinfo_2016_3_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7683 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_2`;
CREATE TABLE `fishlogmonthinfo_2016_3_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=637 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_20`;
CREATE TABLE `fishlogmonthinfo_2016_3_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7666 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_21`;
CREATE TABLE `fishlogmonthinfo_2016_3_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7763 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_22`;
CREATE TABLE `fishlogmonthinfo_2016_3_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7569 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_23`;
CREATE TABLE `fishlogmonthinfo_2016_3_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7873 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_24`;
CREATE TABLE `fishlogmonthinfo_2016_3_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7830 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_25`;
CREATE TABLE `fishlogmonthinfo_2016_3_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8004 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_26`;
CREATE TABLE `fishlogmonthinfo_2016_3_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7707 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_27`;
CREATE TABLE `fishlogmonthinfo_2016_3_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7956 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_28`;
CREATE TABLE `fishlogmonthinfo_2016_3_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7991 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_29`;
CREATE TABLE `fishlogmonthinfo_2016_3_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7978 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_3`;
CREATE TABLE `fishlogmonthinfo_2016_3_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1646 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_30`;
CREATE TABLE `fishlogmonthinfo_2016_3_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7907 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_31
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_31`;
CREATE TABLE `fishlogmonthinfo_2016_3_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7885 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_4`;
CREATE TABLE `fishlogmonthinfo_2016_3_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3587 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_5`;
CREATE TABLE `fishlogmonthinfo_2016_3_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8716 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_6`;
CREATE TABLE `fishlogmonthinfo_2016_3_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4596 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_7`;
CREATE TABLE `fishlogmonthinfo_2016_3_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5754 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_8`;
CREATE TABLE `fishlogmonthinfo_2016_3_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7799 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_3_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_3_9`;
CREATE TABLE `fishlogmonthinfo_2016_3_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7740 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_1`;
CREATE TABLE `fishlogmonthinfo_2016_4_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8119 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_10`;
CREATE TABLE `fishlogmonthinfo_2016_4_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7835 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_11`;
CREATE TABLE `fishlogmonthinfo_2016_4_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8174 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_12`;
CREATE TABLE `fishlogmonthinfo_2016_4_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7979 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_13
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_13`;
CREATE TABLE `fishlogmonthinfo_2016_4_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7920 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_14`;
CREATE TABLE `fishlogmonthinfo_2016_4_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8137 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_15`;
CREATE TABLE `fishlogmonthinfo_2016_4_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7951 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_16`;
CREATE TABLE `fishlogmonthinfo_2016_4_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7872 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_17`;
CREATE TABLE `fishlogmonthinfo_2016_4_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7902 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_18`;
CREATE TABLE `fishlogmonthinfo_2016_4_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2045 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_19`;
CREATE TABLE `fishlogmonthinfo_2016_4_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9639 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_2`;
CREATE TABLE `fishlogmonthinfo_2016_4_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8153 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_20`;
CREATE TABLE `fishlogmonthinfo_2016_4_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9579 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_21`;
CREATE TABLE `fishlogmonthinfo_2016_4_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9508 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_22`;
CREATE TABLE `fishlogmonthinfo_2016_4_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9503 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_23`;
CREATE TABLE `fishlogmonthinfo_2016_4_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9697 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_24`;
CREATE TABLE `fishlogmonthinfo_2016_4_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9649 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_25`;
CREATE TABLE `fishlogmonthinfo_2016_4_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9463 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_26`;
CREATE TABLE `fishlogmonthinfo_2016_4_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9664 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_27`;
CREATE TABLE `fishlogmonthinfo_2016_4_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7109 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_28`;
CREATE TABLE `fishlogmonthinfo_2016_4_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6969 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_29`;
CREATE TABLE `fishlogmonthinfo_2016_4_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9733 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_3`;
CREATE TABLE `fishlogmonthinfo_2016_4_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8005 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_30`;
CREATE TABLE `fishlogmonthinfo_2016_4_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11821 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_4`;
CREATE TABLE `fishlogmonthinfo_2016_4_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8087 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_5`;
CREATE TABLE `fishlogmonthinfo_2016_4_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7998 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_6`;
CREATE TABLE `fishlogmonthinfo_2016_4_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7728 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_7`;
CREATE TABLE `fishlogmonthinfo_2016_4_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7829 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_8`;
CREATE TABLE `fishlogmonthinfo_2016_4_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8083 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_4_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_4_9`;
CREATE TABLE `fishlogmonthinfo_2016_4_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8197 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_1`;
CREATE TABLE `fishlogmonthinfo_2016_5_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11900 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_10`;
CREATE TABLE `fishlogmonthinfo_2016_5_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11779 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_11`;
CREATE TABLE `fishlogmonthinfo_2016_5_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11838 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_12`;
CREATE TABLE `fishlogmonthinfo_2016_5_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11983 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_13
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_13`;
CREATE TABLE `fishlogmonthinfo_2016_5_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11738 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_14`;
CREATE TABLE `fishlogmonthinfo_2016_5_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11928 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_15`;
CREATE TABLE `fishlogmonthinfo_2016_5_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11717 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_16`;
CREATE TABLE `fishlogmonthinfo_2016_5_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11651 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_17`;
CREATE TABLE `fishlogmonthinfo_2016_5_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11688 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_18`;
CREATE TABLE `fishlogmonthinfo_2016_5_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11814 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_19`;
CREATE TABLE `fishlogmonthinfo_2016_5_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11744 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_2`;
CREATE TABLE `fishlogmonthinfo_2016_5_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11731 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_20`;
CREATE TABLE `fishlogmonthinfo_2016_5_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11889 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_21`;
CREATE TABLE `fishlogmonthinfo_2016_5_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11807 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_22`;
CREATE TABLE `fishlogmonthinfo_2016_5_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11989 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_23`;
CREATE TABLE `fishlogmonthinfo_2016_5_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11722 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_24`;
CREATE TABLE `fishlogmonthinfo_2016_5_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11692 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_25`;
CREATE TABLE `fishlogmonthinfo_2016_5_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11753 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_26`;
CREATE TABLE `fishlogmonthinfo_2016_5_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11776 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_27`;
CREATE TABLE `fishlogmonthinfo_2016_5_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11793 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_28`;
CREATE TABLE `fishlogmonthinfo_2016_5_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11838 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_29`;
CREATE TABLE `fishlogmonthinfo_2016_5_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11734 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_3`;
CREATE TABLE `fishlogmonthinfo_2016_5_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11855 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_30`;
CREATE TABLE `fishlogmonthinfo_2016_5_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11951 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_31`;
CREATE TABLE `fishlogmonthinfo_2016_5_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11120 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_4`;
CREATE TABLE `fishlogmonthinfo_2016_5_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11793 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_5`;
CREATE TABLE `fishlogmonthinfo_2016_5_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11804 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_6`;
CREATE TABLE `fishlogmonthinfo_2016_5_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11816 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_7`;
CREATE TABLE `fishlogmonthinfo_2016_5_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9529 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_8`;
CREATE TABLE `fishlogmonthinfo_2016_5_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8227 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_5_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_5_9`;
CREATE TABLE `fishlogmonthinfo_2016_5_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=10536 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlogmonthinfo_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlogmonthinfo_2016_6_1`;
CREATE TABLE `fishlogmonthinfo_2016_6_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `MonthID` tinyint(3) unsigned NOT NULL,
  `MonthIndex` int(10) unsigned NOT NULL,
  `MonthScore` int(10) unsigned NOT NULL,
  `MonthSkillSum` int(10) unsigned NOT NULL,
  `MonthAddGlobelSum` tinyint(3) unsigned NOT NULL,
  `MonthRewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9675 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_10`;
CREATE TABLE `fishlog_2015_12_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_11`;
CREATE TABLE `fishlog_2015_12_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_12`;
CREATE TABLE `fishlog_2015_12_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_14`;
CREATE TABLE `fishlog_2015_12_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_15`;
CREATE TABLE `fishlog_2015_12_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_16`;
CREATE TABLE `fishlog_2015_12_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_17`;
CREATE TABLE `fishlog_2015_12_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_18`;
CREATE TABLE `fishlog_2015_12_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_19`;
CREATE TABLE `fishlog_2015_12_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_20`;
CREATE TABLE `fishlog_2015_12_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_21`;
CREATE TABLE `fishlog_2015_12_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_22`;
CREATE TABLE `fishlog_2015_12_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_23`;
CREATE TABLE `fishlog_2015_12_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_24`;
CREATE TABLE `fishlog_2015_12_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_25`;
CREATE TABLE `fishlog_2015_12_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_26`;
CREATE TABLE `fishlog_2015_12_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_27`;
CREATE TABLE `fishlog_2015_12_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_28`;
CREATE TABLE `fishlog_2015_12_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_29`;
CREATE TABLE `fishlog_2015_12_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_30`;
CREATE TABLE `fishlog_2015_12_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_31
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_31`;
CREATE TABLE `fishlog_2015_12_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_5`;
CREATE TABLE `fishlog_2015_12_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_6`;
CREATE TABLE `fishlog_2015_12_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_7`;
CREATE TABLE `fishlog_2015_12_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_8`;
CREATE TABLE `fishlog_2015_12_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2015_12_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2015_12_9`;
CREATE TABLE `fishlog_2015_12_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_1`;
CREATE TABLE `fishlog_2016_1_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_10`;
CREATE TABLE `fishlog_2016_1_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_11`;
CREATE TABLE `fishlog_2016_1_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_12`;
CREATE TABLE `fishlog_2016_1_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_13
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_13`;
CREATE TABLE `fishlog_2016_1_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_14`;
CREATE TABLE `fishlog_2016_1_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_15`;
CREATE TABLE `fishlog_2016_1_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_16`;
CREATE TABLE `fishlog_2016_1_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_17`;
CREATE TABLE `fishlog_2016_1_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_18`;
CREATE TABLE `fishlog_2016_1_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_19`;
CREATE TABLE `fishlog_2016_1_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_2`;
CREATE TABLE `fishlog_2016_1_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_20`;
CREATE TABLE `fishlog_2016_1_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_21`;
CREATE TABLE `fishlog_2016_1_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_22`;
CREATE TABLE `fishlog_2016_1_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=482 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_23`;
CREATE TABLE `fishlog_2016_1_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1160 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_24`;
CREATE TABLE `fishlog_2016_1_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2877 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_25`;
CREATE TABLE `fishlog_2016_1_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1664 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_26`;
CREATE TABLE `fishlog_2016_1_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_27`;
CREATE TABLE `fishlog_2016_1_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_28`;
CREATE TABLE `fishlog_2016_1_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_29`;
CREATE TABLE `fishlog_2016_1_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_3`;
CREATE TABLE `fishlog_2016_1_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_30`;
CREATE TABLE `fishlog_2016_1_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=366 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_31
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_31`;
CREATE TABLE `fishlog_2016_1_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=108 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_4`;
CREATE TABLE `fishlog_2016_1_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_5`;
CREATE TABLE `fishlog_2016_1_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_6`;
CREATE TABLE `fishlog_2016_1_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_7`;
CREATE TABLE `fishlog_2016_1_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_8`;
CREATE TABLE `fishlog_2016_1_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_1_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_1_9`;
CREATE TABLE `fishlog_2016_1_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_1`;
CREATE TABLE `fishlog_2016_2_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=236 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_10`;
CREATE TABLE `fishlog_2016_2_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_11`;
CREATE TABLE `fishlog_2016_2_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_12`;
CREATE TABLE `fishlog_2016_2_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_15`;
CREATE TABLE `fishlog_2016_2_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_16`;
CREATE TABLE `fishlog_2016_2_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_17`;
CREATE TABLE `fishlog_2016_2_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=48 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_18`;
CREATE TABLE `fishlog_2016_2_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_19`;
CREATE TABLE `fishlog_2016_2_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_2`;
CREATE TABLE `fishlog_2016_2_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_20`;
CREATE TABLE `fishlog_2016_2_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_21`;
CREATE TABLE `fishlog_2016_2_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_22`;
CREATE TABLE `fishlog_2016_2_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_23`;
CREATE TABLE `fishlog_2016_2_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_24`;
CREATE TABLE `fishlog_2016_2_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=91 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_25`;
CREATE TABLE `fishlog_2016_2_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=102 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_26`;
CREATE TABLE `fishlog_2016_2_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=57 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_27`;
CREATE TABLE `fishlog_2016_2_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=44 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_28`;
CREATE TABLE `fishlog_2016_2_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_29`;
CREATE TABLE `fishlog_2016_2_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=109 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_3`;
CREATE TABLE `fishlog_2016_2_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_4`;
CREATE TABLE `fishlog_2016_2_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=537 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_5`;
CREATE TABLE `fishlog_2016_2_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=67 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_6`;
CREATE TABLE `fishlog_2016_2_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=41 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_7`;
CREATE TABLE `fishlog_2016_2_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_8`;
CREATE TABLE `fishlog_2016_2_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=49 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_2_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_2_9`;
CREATE TABLE `fishlog_2016_2_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=326 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_1`;
CREATE TABLE `fishlog_2016_3_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=694 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_10`;
CREATE TABLE `fishlog_2016_3_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=95483 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_11`;
CREATE TABLE `fishlog_2016_3_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=126362 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_12`;
CREATE TABLE `fishlog_2016_3_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=108999 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_13
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_13`;
CREATE TABLE `fishlog_2016_3_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=114238 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_14`;
CREATE TABLE `fishlog_2016_3_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=123365 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_15`;
CREATE TABLE `fishlog_2016_3_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=68001 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_16`;
CREATE TABLE `fishlog_2016_3_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=89 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_17`;
CREATE TABLE `fishlog_2016_3_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=645 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_18`;
CREATE TABLE `fishlog_2016_3_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=984 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_19`;
CREATE TABLE `fishlog_2016_3_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1148 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_2`;
CREATE TABLE `fishlog_2016_3_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_20`;
CREATE TABLE `fishlog_2016_3_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1308 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_21`;
CREATE TABLE `fishlog_2016_3_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1392 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_22`;
CREATE TABLE `fishlog_2016_3_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1379 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_23`;
CREATE TABLE `fishlog_2016_3_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1920 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_24`;
CREATE TABLE `fishlog_2016_3_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1955 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_25`;
CREATE TABLE `fishlog_2016_3_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2406 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_26`;
CREATE TABLE `fishlog_2016_3_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2193 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_27`;
CREATE TABLE `fishlog_2016_3_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2251 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_28`;
CREATE TABLE `fishlog_2016_3_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2383 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_29`;
CREATE TABLE `fishlog_2016_3_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2122 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_3`;
CREATE TABLE `fishlog_2016_3_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_30`;
CREATE TABLE `fishlog_2016_3_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2265 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_31
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_31`;
CREATE TABLE `fishlog_2016_3_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2502 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_4`;
CREATE TABLE `fishlog_2016_3_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=216 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_5`;
CREATE TABLE `fishlog_2016_3_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_6`;
CREATE TABLE `fishlog_2016_3_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_7`;
CREATE TABLE `fishlog_2016_3_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=175251 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_8`;
CREATE TABLE `fishlog_2016_3_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=81293 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_3_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_3_9`;
CREATE TABLE `fishlog_2016_3_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=84695 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_1`;
CREATE TABLE `fishlog_2016_4_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2964 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_10`;
CREATE TABLE `fishlog_2016_4_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2841 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_11`;
CREATE TABLE `fishlog_2016_4_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2965 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_12`;
CREATE TABLE `fishlog_2016_4_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2765 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_13
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_13`;
CREATE TABLE `fishlog_2016_4_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3020 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_14`;
CREATE TABLE `fishlog_2016_4_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2854 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_15`;
CREATE TABLE `fishlog_2016_4_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2760 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_16`;
CREATE TABLE `fishlog_2016_4_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2507 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_17`;
CREATE TABLE `fishlog_2016_4_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2613 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_18`;
CREATE TABLE `fishlog_2016_4_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=953 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_19`;
CREATE TABLE `fishlog_2016_4_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=39424 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_2`;
CREATE TABLE `fishlog_2016_4_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2926 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_20`;
CREATE TABLE `fishlog_2016_4_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=36628 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_21`;
CREATE TABLE `fishlog_2016_4_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=36639 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_22`;
CREATE TABLE `fishlog_2016_4_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=36547 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_23`;
CREATE TABLE `fishlog_2016_4_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=36749 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_24`;
CREATE TABLE `fishlog_2016_4_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=36354 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_25`;
CREATE TABLE `fishlog_2016_4_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=36691 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_26`;
CREATE TABLE `fishlog_2016_4_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=36459 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_27`;
CREATE TABLE `fishlog_2016_4_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3765 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_28`;
CREATE TABLE `fishlog_2016_4_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2623 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_29`;
CREATE TABLE `fishlog_2016_4_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5208 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_3`;
CREATE TABLE `fishlog_2016_4_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2964 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_30`;
CREATE TABLE `fishlog_2016_4_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4847 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_4`;
CREATE TABLE `fishlog_2016_4_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2930 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_5`;
CREATE TABLE `fishlog_2016_4_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2768 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_6`;
CREATE TABLE `fishlog_2016_4_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2362 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_7`;
CREATE TABLE `fishlog_2016_4_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2447 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_8`;
CREATE TABLE `fishlog_2016_4_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2643 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_4_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_4_9`;
CREATE TABLE `fishlog_2016_4_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3058 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_1`;
CREATE TABLE `fishlog_2016_5_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4872 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_10
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_10`;
CREATE TABLE `fishlog_2016_5_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4821 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_11
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_11`;
CREATE TABLE `fishlog_2016_5_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4807 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_12
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_12`;
CREATE TABLE `fishlog_2016_5_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4814 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_13
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_13`;
CREATE TABLE `fishlog_2016_5_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4823 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_14
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_14`;
CREATE TABLE `fishlog_2016_5_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4831 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_15
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_15`;
CREATE TABLE `fishlog_2016_5_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4907 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_16
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_16`;
CREATE TABLE `fishlog_2016_5_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4821 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_17
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_17`;
CREATE TABLE `fishlog_2016_5_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4872 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_18
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_18`;
CREATE TABLE `fishlog_2016_5_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4935 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_19
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_19`;
CREATE TABLE `fishlog_2016_5_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4875 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_2
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_2`;
CREATE TABLE `fishlog_2016_5_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4829 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_20
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_20`;
CREATE TABLE `fishlog_2016_5_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4876 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_21
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_21`;
CREATE TABLE `fishlog_2016_5_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4859 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_22
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_22`;
CREATE TABLE `fishlog_2016_5_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4876 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_23
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_23`;
CREATE TABLE `fishlog_2016_5_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4949 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_24
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_24`;
CREATE TABLE `fishlog_2016_5_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4918 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_25
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_25`;
CREATE TABLE `fishlog_2016_5_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5040 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_26`;
CREATE TABLE `fishlog_2016_5_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7884 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_27`;
CREATE TABLE `fishlog_2016_5_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6678 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_28`;
CREATE TABLE `fishlog_2016_5_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7336 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_29`;
CREATE TABLE `fishlog_2016_5_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6586 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_3
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_3`;
CREATE TABLE `fishlog_2016_5_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4809 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_30`;
CREATE TABLE `fishlog_2016_5_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7149 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_31`;
CREATE TABLE `fishlog_2016_5_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=13870 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_4
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_4`;
CREATE TABLE `fishlog_2016_5_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4899 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_5
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_5`;
CREATE TABLE `fishlog_2016_5_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4855 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_6
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_6`;
CREATE TABLE `fishlog_2016_5_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4907 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_7
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_7`;
CREATE TABLE `fishlog_2016_5_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1842 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_8
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_8`;
CREATE TABLE `fishlog_2016_5_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_5_9
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_5_9`;
CREATE TABLE `fishlog_2016_5_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2986 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlog_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishlog_2016_6_1`;
CREATE TABLE `fishlog_2016_6_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `TypeID` int(10) unsigned NOT NULL COMMENT '1表示 乐币 2表示 奖牌 3表示 奖励',
  `TypeSum` int(10) NOT NULL,
  `Param` int(10) unsigned NOT NULL DEFAULT '0',
  `Info` varchar(256) NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5410 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishlotteryinfo
-- ----------------------------
DROP TABLE IF EXISTS `fishlotteryinfo`;
CREATE TABLE `fishlotteryinfo` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `LotteryID` tinyint(3) unsigned NOT NULL,
  `RewardID` smallint(5) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishmaillog
-- ----------------------------
DROP TABLE IF EXISTS `fishmaillog`;
CREATE TABLE `fishmaillog` (
  `MailID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `SrcUserID` int(11) unsigned NOT NULL,
  `DestUserID` int(11) unsigned NOT NULL,
  `Context` varchar(65) NOT NULL,
  `IsRead` bit(1) NOT NULL DEFAULT b'0',
  `SendTime` datetime NOT NULL,
  `RewardID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `IsExistsReward` bit(1) NOT NULL DEFAULT b'0',
  PRIMARY KEY (`MailID`),
  KEY `Mail_Dest_Primary` (`DestUserID`),
  KEY `Mail_Src_Primary` (`SrcUserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo`;
CREATE TABLE `fishniuniutableinfo` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=88756 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_1_26
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_1_26`;
CREATE TABLE `fishniuniutableinfo_2016_1_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_1_27
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_1_27`;
CREATE TABLE `fishniuniutableinfo_2016_1_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_1_28
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_1_28`;
CREATE TABLE `fishniuniutableinfo_2016_1_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_1_29
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_1_29`;
CREATE TABLE `fishniuniutableinfo_2016_1_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_1_30
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_1_30`;
CREATE TABLE `fishniuniutableinfo_2016_1_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=41 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_1
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_1`;
CREATE TABLE `fishniuniutableinfo_2016_2_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=46 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_10
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_10`;
CREATE TABLE `fishniuniutableinfo_2016_2_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_11
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_11`;
CREATE TABLE `fishniuniutableinfo_2016_2_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_12
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_12`;
CREATE TABLE `fishniuniutableinfo_2016_2_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_15
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_15`;
CREATE TABLE `fishniuniutableinfo_2016_2_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_16
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_16`;
CREATE TABLE `fishniuniutableinfo_2016_2_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_17
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_17`;
CREATE TABLE `fishniuniutableinfo_2016_2_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=107 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_18
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_18`;
CREATE TABLE `fishniuniutableinfo_2016_2_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=144 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_19
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_19`;
CREATE TABLE `fishniuniutableinfo_2016_2_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_2
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_2`;
CREATE TABLE `fishniuniutableinfo_2016_2_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_20
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_20`;
CREATE TABLE `fishniuniutableinfo_2016_2_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_21
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_21`;
CREATE TABLE `fishniuniutableinfo_2016_2_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_22
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_22`;
CREATE TABLE `fishniuniutableinfo_2016_2_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_23
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_23`;
CREATE TABLE `fishniuniutableinfo_2016_2_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=79 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_24
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_24`;
CREATE TABLE `fishniuniutableinfo_2016_2_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=337 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_25
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_25`;
CREATE TABLE `fishniuniutableinfo_2016_2_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=63 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_26
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_26`;
CREATE TABLE `fishniuniutableinfo_2016_2_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=361 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_27
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_27`;
CREATE TABLE `fishniuniutableinfo_2016_2_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=116 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_28
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_28`;
CREATE TABLE `fishniuniutableinfo_2016_2_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_29
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_29`;
CREATE TABLE `fishniuniutableinfo_2016_2_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_3
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_3`;
CREATE TABLE `fishniuniutableinfo_2016_2_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_4
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_4`;
CREATE TABLE `fishniuniutableinfo_2016_2_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_5
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_5`;
CREATE TABLE `fishniuniutableinfo_2016_2_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_6
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_6`;
CREATE TABLE `fishniuniutableinfo_2016_2_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_7
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_7`;
CREATE TABLE `fishniuniutableinfo_2016_2_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_8
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_8`;
CREATE TABLE `fishniuniutableinfo_2016_2_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_2_9
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_2_9`;
CREATE TABLE `fishniuniutableinfo_2016_2_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_1
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_1`;
CREATE TABLE `fishniuniutableinfo_2016_3_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_10
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_10`;
CREATE TABLE `fishniuniutableinfo_2016_3_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_11
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_11`;
CREATE TABLE `fishniuniutableinfo_2016_3_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=415 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_12
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_12`;
CREATE TABLE `fishniuniutableinfo_2016_3_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=415 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_13
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_13`;
CREATE TABLE `fishniuniutableinfo_2016_3_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_14
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_14`;
CREATE TABLE `fishniuniutableinfo_2016_3_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_15
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_15`;
CREATE TABLE `fishniuniutableinfo_2016_3_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=500 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_16
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_16`;
CREATE TABLE `fishniuniutableinfo_2016_3_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_17
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_17`;
CREATE TABLE `fishniuniutableinfo_2016_3_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=295 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_18
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_18`;
CREATE TABLE `fishniuniutableinfo_2016_3_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=408 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_19
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_19`;
CREATE TABLE `fishniuniutableinfo_2016_3_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=415 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_2
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_2`;
CREATE TABLE `fishniuniutableinfo_2016_3_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_20
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_20`;
CREATE TABLE `fishniuniutableinfo_2016_3_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_21
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_21`;
CREATE TABLE `fishniuniutableinfo_2016_3_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=503 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_22
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_22`;
CREATE TABLE `fishniuniutableinfo_2016_3_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_23
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_23`;
CREATE TABLE `fishniuniutableinfo_2016_3_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=415 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_24
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_24`;
CREATE TABLE `fishniuniutableinfo_2016_3_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_25
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_25`;
CREATE TABLE `fishniuniutableinfo_2016_3_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_26
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_26`;
CREATE TABLE `fishniuniutableinfo_2016_3_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_27
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_27`;
CREATE TABLE `fishniuniutableinfo_2016_3_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=413 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_28
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_28`;
CREATE TABLE `fishniuniutableinfo_2016_3_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=415 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_29
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_29`;
CREATE TABLE `fishniuniutableinfo_2016_3_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_3
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_3`;
CREATE TABLE `fishniuniutableinfo_2016_3_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_30
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_30`;
CREATE TABLE `fishniuniutableinfo_2016_3_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_31
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_31`;
CREATE TABLE `fishniuniutableinfo_2016_3_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=413 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_4
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_4`;
CREATE TABLE `fishniuniutableinfo_2016_3_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=53 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_5
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_5`;
CREATE TABLE `fishniuniutableinfo_2016_3_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_6
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_6`;
CREATE TABLE `fishniuniutableinfo_2016_3_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_7
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_7`;
CREATE TABLE `fishniuniutableinfo_2016_3_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1022 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_8
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_8`;
CREATE TABLE `fishniuniutableinfo_2016_3_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=513 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_3_9
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_3_9`;
CREATE TABLE `fishniuniutableinfo_2016_3_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=415 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_1
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_1`;
CREATE TABLE `fishniuniutableinfo_2016_4_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=411 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_10
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_10`;
CREATE TABLE `fishniuniutableinfo_2016_4_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=412 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_11
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_11`;
CREATE TABLE `fishniuniutableinfo_2016_4_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=413 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_12
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_12`;
CREATE TABLE `fishniuniutableinfo_2016_4_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=413 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_13
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_13`;
CREATE TABLE `fishniuniutableinfo_2016_4_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_14
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_14`;
CREATE TABLE `fishniuniutableinfo_2016_4_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=412 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_15
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_15`;
CREATE TABLE `fishniuniutableinfo_2016_4_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=410 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_16
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_16`;
CREATE TABLE `fishniuniutableinfo_2016_4_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=410 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_17
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_17`;
CREATE TABLE `fishniuniutableinfo_2016_4_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=412 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_18
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_18`;
CREATE TABLE `fishniuniutableinfo_2016_4_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=390 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_19
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_19`;
CREATE TABLE `fishniuniutableinfo_2016_4_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2108 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_2
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_2`;
CREATE TABLE `fishniuniutableinfo_2016_4_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_20
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_20`;
CREATE TABLE `fishniuniutableinfo_2016_4_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2114 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_21
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_21`;
CREATE TABLE `fishniuniutableinfo_2016_4_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2115 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_22
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_22`;
CREATE TABLE `fishniuniutableinfo_2016_4_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2114 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_23
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_23`;
CREATE TABLE `fishniuniutableinfo_2016_4_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2114 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_24
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_24`;
CREATE TABLE `fishniuniutableinfo_2016_4_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2113 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_25
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_25`;
CREATE TABLE `fishniuniutableinfo_2016_4_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2117 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_26
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_26`;
CREATE TABLE `fishniuniutableinfo_2016_4_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2116 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_27
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_27`;
CREATE TABLE `fishniuniutableinfo_2016_4_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=239 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_28
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_28`;
CREATE TABLE `fishniuniutableinfo_2016_4_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_29
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_29`;
CREATE TABLE `fishniuniutableinfo_2016_4_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1195 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_3
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_3`;
CREATE TABLE `fishniuniutableinfo_2016_4_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=412 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_30
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_30`;
CREATE TABLE `fishniuniutableinfo_2016_4_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2102 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_4
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_4`;
CREATE TABLE `fishniuniutableinfo_2016_4_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=414 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_5
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_5`;
CREATE TABLE `fishniuniutableinfo_2016_4_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=413 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_6
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_6`;
CREATE TABLE `fishniuniutableinfo_2016_4_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=410 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_7
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_7`;
CREATE TABLE `fishniuniutableinfo_2016_4_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=415 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_8
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_8`;
CREATE TABLE `fishniuniutableinfo_2016_4_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=413 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_4_9
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_4_9`;
CREATE TABLE `fishniuniutableinfo_2016_4_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=413 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_1
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_1`;
CREATE TABLE `fishniuniutableinfo_2016_5_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_10
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_10`;
CREATE TABLE `fishniuniutableinfo_2016_5_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2102 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_11
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_11`;
CREATE TABLE `fishniuniutableinfo_2016_5_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_12
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_12`;
CREATE TABLE `fishniuniutableinfo_2016_5_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2103 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_13
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_13`;
CREATE TABLE `fishniuniutableinfo_2016_5_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_14
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_14`;
CREATE TABLE `fishniuniutableinfo_2016_5_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2103 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_15
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_15`;
CREATE TABLE `fishniuniutableinfo_2016_5_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2102 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_16
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_16`;
CREATE TABLE `fishniuniutableinfo_2016_5_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2088 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_17
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_17`;
CREATE TABLE `fishniuniutableinfo_2016_5_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_18
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_18`;
CREATE TABLE `fishniuniutableinfo_2016_5_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2102 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_19
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_19`;
CREATE TABLE `fishniuniutableinfo_2016_5_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2103 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_2
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_2`;
CREATE TABLE `fishniuniutableinfo_2016_5_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_20
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_20`;
CREATE TABLE `fishniuniutableinfo_2016_5_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2103 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_21
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_21`;
CREATE TABLE `fishniuniutableinfo_2016_5_21` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2102 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_22
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_22`;
CREATE TABLE `fishniuniutableinfo_2016_5_22` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2103 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_23
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_23`;
CREATE TABLE `fishniuniutableinfo_2016_5_23` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_24
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_24`;
CREATE TABLE `fishniuniutableinfo_2016_5_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2103 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_25
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_25`;
CREATE TABLE `fishniuniutableinfo_2016_5_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2090 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_26`;
CREATE TABLE `fishniuniutableinfo_2016_5_26` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2100 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_27`;
CREATE TABLE `fishniuniutableinfo_2016_5_27` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2099 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_28`;
CREATE TABLE `fishniuniutableinfo_2016_5_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2099 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_29`;
CREATE TABLE `fishniuniutableinfo_2016_5_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2099 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_3
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_3`;
CREATE TABLE `fishniuniutableinfo_2016_5_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2102 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_30`;
CREATE TABLE `fishniuniutableinfo_2016_5_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2094 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_31`;
CREATE TABLE `fishniuniutableinfo_2016_5_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2046 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_4
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_4`;
CREATE TABLE `fishniuniutableinfo_2016_5_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_5
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_5`;
CREATE TABLE `fishniuniutableinfo_2016_5_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_6
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_6`;
CREATE TABLE `fishniuniutableinfo_2016_5_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2101 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_7
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_7`;
CREATE TABLE `fishniuniutableinfo_2016_5_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=781 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_5_9
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_5_9`;
CREATE TABLE `fishniuniutableinfo_2016_5_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1289 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishniuniutableinfo_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishniuniutableinfo_2016_6_1`;
CREATE TABLE `fishniuniutableinfo_2016_6_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `BrandArray1` varchar(32) NOT NULL,
  `BrandArray2` varchar(32) NOT NULL,
  `BrandArray3` varchar(32) NOT NULL,
  `BrandArray4` varchar(32) NOT NULL,
  `BrandArray5` varchar(32) NOT NULL,
  `ChangeGlobelSum` bigint(20) NOT NULL DEFAULT '0',
  `SystemGlobelSum` bigint(20) NOT NULL,
  `LogTime` datetime NOT NULL,
  `UserSum` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1725 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog`;
CREATE TABLE `fishphonepaylog` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `OrderID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `OrderStates` varchar(256) NOT NULL DEFAULT '',
  `FacePrice` int(10) unsigned NOT NULL DEFAULT '0',
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2015_12_28
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2015_12_28`;
CREATE TABLE `fishphonepaylog_2015_12_28` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2015_12_30
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2015_12_30`;
CREATE TABLE `fishphonepaylog_2015_12_30` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2015_12_31
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2015_12_31`;
CREATE TABLE `fishphonepaylog_2015_12_31` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_1
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_1`;
CREATE TABLE `fishphonepaylog_2016_1_1` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_10
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_10`;
CREATE TABLE `fishphonepaylog_2016_1_10` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_11
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_11`;
CREATE TABLE `fishphonepaylog_2016_1_11` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_12
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_12`;
CREATE TABLE `fishphonepaylog_2016_1_12` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_13
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_13`;
CREATE TABLE `fishphonepaylog_2016_1_13` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_14
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_14`;
CREATE TABLE `fishphonepaylog_2016_1_14` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_15
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_15`;
CREATE TABLE `fishphonepaylog_2016_1_15` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_16
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_16`;
CREATE TABLE `fishphonepaylog_2016_1_16` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_17
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_17`;
CREATE TABLE `fishphonepaylog_2016_1_17` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_18
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_18`;
CREATE TABLE `fishphonepaylog_2016_1_18` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_19
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_19`;
CREATE TABLE `fishphonepaylog_2016_1_19` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_2
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_2`;
CREATE TABLE `fishphonepaylog_2016_1_2` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_20
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_20`;
CREATE TABLE `fishphonepaylog_2016_1_20` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_3
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_3`;
CREATE TABLE `fishphonepaylog_2016_1_3` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_4
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_4`;
CREATE TABLE `fishphonepaylog_2016_1_4` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_5
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_5`;
CREATE TABLE `fishphonepaylog_2016_1_5` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_6
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_6`;
CREATE TABLE `fishphonepaylog_2016_1_6` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_7
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_7`;
CREATE TABLE `fishphonepaylog_2016_1_7` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_8
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_8`;
CREATE TABLE `fishphonepaylog_2016_1_8` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishphonepaylog_2016_1_9
-- ----------------------------
DROP TABLE IF EXISTS `fishphonepaylog_2016_1_9`;
CREATE TABLE `fishphonepaylog_2016_1_9` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog`;
CREATE TABLE `fishrechargelog` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `OrderStates` varchar(256) NOT NULL DEFAULT '',
  `UserID` int(10) unsigned NOT NULL,
  `Price` int(10) unsigned NOT NULL DEFAULT '0',
  `FreePrice` int(10) unsigned NOT NULL DEFAULT '0',
  `OldGlobelNum` int(10) unsigned NOT NULL DEFAULT '0',
  `OldCurrceyNum` int(10) unsigned NOT NULL DEFAULT '0',
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ShopItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  `AddRewardID` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=207 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_10
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_10`;
CREATE TABLE `fishrechargelog_2015_12_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_15
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_15`;
CREATE TABLE `fishrechargelog_2015_12_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_24
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_24`;
CREATE TABLE `fishrechargelog_2015_12_24` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_25
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_25`;
CREATE TABLE `fishrechargelog_2015_12_25` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_28
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_28`;
CREATE TABLE `fishrechargelog_2015_12_28` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_29
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_29`;
CREATE TABLE `fishrechargelog_2015_12_29` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_30
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_30`;
CREATE TABLE `fishrechargelog_2015_12_30` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_31
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_31`;
CREATE TABLE `fishrechargelog_2015_12_31` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2015_12_8
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2015_12_8`;
CREATE TABLE `fishrechargelog_2015_12_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_1
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_1`;
CREATE TABLE `fishrechargelog_2016_1_1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_10
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_10`;
CREATE TABLE `fishrechargelog_2016_1_10` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_11
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_11`;
CREATE TABLE `fishrechargelog_2016_1_11` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_12
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_12`;
CREATE TABLE `fishrechargelog_2016_1_12` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_13
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_13`;
CREATE TABLE `fishrechargelog_2016_1_13` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_14
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_14`;
CREATE TABLE `fishrechargelog_2016_1_14` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_15
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_15`;
CREATE TABLE `fishrechargelog_2016_1_15` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_16
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_16`;
CREATE TABLE `fishrechargelog_2016_1_16` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_17
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_17`;
CREATE TABLE `fishrechargelog_2016_1_17` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_18
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_18`;
CREATE TABLE `fishrechargelog_2016_1_18` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_19
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_19`;
CREATE TABLE `fishrechargelog_2016_1_19` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_2
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_2`;
CREATE TABLE `fishrechargelog_2016_1_2` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_20
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_20`;
CREATE TABLE `fishrechargelog_2016_1_20` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_3
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_3`;
CREATE TABLE `fishrechargelog_2016_1_3` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_4
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_4`;
CREATE TABLE `fishrechargelog_2016_1_4` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_5
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_5`;
CREATE TABLE `fishrechargelog_2016_1_5` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_6
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_6`;
CREATE TABLE `fishrechargelog_2016_1_6` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_7
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_7`;
CREATE TABLE `fishrechargelog_2016_1_7` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_8
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_8`;
CREATE TABLE `fishrechargelog_2016_1_8` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishrechargelog_2016_1_9
-- ----------------------------
DROP TABLE IF EXISTS `fishrechargelog_2016_1_9`;
CREATE TABLE `fishrechargelog_2016_1_9` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog`;
CREATE TABLE `fishroleonlinelog` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2015_12_28
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2015_12_28`;
CREATE TABLE `fishroleonlinelog_2015_12_28` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2015_12_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2015_12_29`;
CREATE TABLE `fishroleonlinelog_2015_12_29` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2015_12_30
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2015_12_30`;
CREATE TABLE `fishroleonlinelog_2015_12_30` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2015_12_31
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2015_12_31`;
CREATE TABLE `fishroleonlinelog_2015_12_31` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_1`;
CREATE TABLE `fishroleonlinelog_2016_1_1` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_10
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_10`;
CREATE TABLE `fishroleonlinelog_2016_1_10` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_11`;
CREATE TABLE `fishroleonlinelog_2016_1_11` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_12`;
CREATE TABLE `fishroleonlinelog_2016_1_12` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_13
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_13`;
CREATE TABLE `fishroleonlinelog_2016_1_13` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_14
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_14`;
CREATE TABLE `fishroleonlinelog_2016_1_14` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_15`;
CREATE TABLE `fishroleonlinelog_2016_1_15` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_16`;
CREATE TABLE `fishroleonlinelog_2016_1_16` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_17`;
CREATE TABLE `fishroleonlinelog_2016_1_17` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_18`;
CREATE TABLE `fishroleonlinelog_2016_1_18` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_19`;
CREATE TABLE `fishroleonlinelog_2016_1_19` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_2`;
CREATE TABLE `fishroleonlinelog_2016_1_2` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_20`;
CREATE TABLE `fishroleonlinelog_2016_1_20` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_21`;
CREATE TABLE `fishroleonlinelog_2016_1_21` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_22`;
CREATE TABLE `fishroleonlinelog_2016_1_22` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_23`;
CREATE TABLE `fishroleonlinelog_2016_1_23` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_24`;
CREATE TABLE `fishroleonlinelog_2016_1_24` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_25`;
CREATE TABLE `fishroleonlinelog_2016_1_25` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_26`;
CREATE TABLE `fishroleonlinelog_2016_1_26` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_27`;
CREATE TABLE `fishroleonlinelog_2016_1_27` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_28
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_28`;
CREATE TABLE `fishroleonlinelog_2016_1_28` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_29`;
CREATE TABLE `fishroleonlinelog_2016_1_29` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_3
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_3`;
CREATE TABLE `fishroleonlinelog_2016_1_3` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_30
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_30`;
CREATE TABLE `fishroleonlinelog_2016_1_30` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_31
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_31`;
CREATE TABLE `fishroleonlinelog_2016_1_31` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_4`;
CREATE TABLE `fishroleonlinelog_2016_1_4` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_5`;
CREATE TABLE `fishroleonlinelog_2016_1_5` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_6`;
CREATE TABLE `fishroleonlinelog_2016_1_6` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_7`;
CREATE TABLE `fishroleonlinelog_2016_1_7` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_8
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_8`;
CREATE TABLE `fishroleonlinelog_2016_1_8` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_1_9
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_1_9`;
CREATE TABLE `fishroleonlinelog_2016_1_9` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_1`;
CREATE TABLE `fishroleonlinelog_2016_2_1` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_10
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_10`;
CREATE TABLE `fishroleonlinelog_2016_2_10` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_11`;
CREATE TABLE `fishroleonlinelog_2016_2_11` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_12`;
CREATE TABLE `fishroleonlinelog_2016_2_12` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_15`;
CREATE TABLE `fishroleonlinelog_2016_2_15` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_16`;
CREATE TABLE `fishroleonlinelog_2016_2_16` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_17`;
CREATE TABLE `fishroleonlinelog_2016_2_17` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_18`;
CREATE TABLE `fishroleonlinelog_2016_2_18` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_19`;
CREATE TABLE `fishroleonlinelog_2016_2_19` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_2`;
CREATE TABLE `fishroleonlinelog_2016_2_2` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_20`;
CREATE TABLE `fishroleonlinelog_2016_2_20` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_21`;
CREATE TABLE `fishroleonlinelog_2016_2_21` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_22`;
CREATE TABLE `fishroleonlinelog_2016_2_22` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_23`;
CREATE TABLE `fishroleonlinelog_2016_2_23` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_24`;
CREATE TABLE `fishroleonlinelog_2016_2_24` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_25`;
CREATE TABLE `fishroleonlinelog_2016_2_25` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_26`;
CREATE TABLE `fishroleonlinelog_2016_2_26` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_27`;
CREATE TABLE `fishroleonlinelog_2016_2_27` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_28
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_28`;
CREATE TABLE `fishroleonlinelog_2016_2_28` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_29`;
CREATE TABLE `fishroleonlinelog_2016_2_29` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_3
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_3`;
CREATE TABLE `fishroleonlinelog_2016_2_3` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_4`;
CREATE TABLE `fishroleonlinelog_2016_2_4` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_5`;
CREATE TABLE `fishroleonlinelog_2016_2_5` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_6`;
CREATE TABLE `fishroleonlinelog_2016_2_6` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_7`;
CREATE TABLE `fishroleonlinelog_2016_2_7` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_8
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_8`;
CREATE TABLE `fishroleonlinelog_2016_2_8` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_2_9
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_2_9`;
CREATE TABLE `fishroleonlinelog_2016_2_9` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_1`;
CREATE TABLE `fishroleonlinelog_2016_3_1` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_11`;
CREATE TABLE `fishroleonlinelog_2016_3_11` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_15`;
CREATE TABLE `fishroleonlinelog_2016_3_15` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_16`;
CREATE TABLE `fishroleonlinelog_2016_3_16` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_17`;
CREATE TABLE `fishroleonlinelog_2016_3_17` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_18`;
CREATE TABLE `fishroleonlinelog_2016_3_18` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_2`;
CREATE TABLE `fishroleonlinelog_2016_3_2` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_21`;
CREATE TABLE `fishroleonlinelog_2016_3_21` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_25`;
CREATE TABLE `fishroleonlinelog_2016_3_25` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_27`;
CREATE TABLE `fishroleonlinelog_2016_3_27` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_3
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_3`;
CREATE TABLE `fishroleonlinelog_2016_3_3` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_31
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_31`;
CREATE TABLE `fishroleonlinelog_2016_3_31` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_4`;
CREATE TABLE `fishroleonlinelog_2016_3_4` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_5`;
CREATE TABLE `fishroleonlinelog_2016_3_5` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_6`;
CREATE TABLE `fishroleonlinelog_2016_3_6` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_7`;
CREATE TABLE `fishroleonlinelog_2016_3_7` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_3_8
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_3_8`;
CREATE TABLE `fishroleonlinelog_2016_3_8` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_1`;
CREATE TABLE `fishroleonlinelog_2016_4_1` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_11`;
CREATE TABLE `fishroleonlinelog_2016_4_11` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_12`;
CREATE TABLE `fishroleonlinelog_2016_4_12` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_13
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_13`;
CREATE TABLE `fishroleonlinelog_2016_4_13` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_15`;
CREATE TABLE `fishroleonlinelog_2016_4_15` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_16`;
CREATE TABLE `fishroleonlinelog_2016_4_16` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_17`;
CREATE TABLE `fishroleonlinelog_2016_4_17` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_18`;
CREATE TABLE `fishroleonlinelog_2016_4_18` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_19`;
CREATE TABLE `fishroleonlinelog_2016_4_19` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_2`;
CREATE TABLE `fishroleonlinelog_2016_4_2` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_20`;
CREATE TABLE `fishroleonlinelog_2016_4_20` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_21`;
CREATE TABLE `fishroleonlinelog_2016_4_21` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_22`;
CREATE TABLE `fishroleonlinelog_2016_4_22` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_23`;
CREATE TABLE `fishroleonlinelog_2016_4_23` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_24`;
CREATE TABLE `fishroleonlinelog_2016_4_24` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_25`;
CREATE TABLE `fishroleonlinelog_2016_4_25` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_26`;
CREATE TABLE `fishroleonlinelog_2016_4_26` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_27`;
CREATE TABLE `fishroleonlinelog_2016_4_27` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_29`;
CREATE TABLE `fishroleonlinelog_2016_4_29` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_30
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_30`;
CREATE TABLE `fishroleonlinelog_2016_4_30` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_4`;
CREATE TABLE `fishroleonlinelog_2016_4_4` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_4_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_4_6`;
CREATE TABLE `fishroleonlinelog_2016_4_6` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_1`;
CREATE TABLE `fishroleonlinelog_2016_5_1` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_10
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_10`;
CREATE TABLE `fishroleonlinelog_2016_5_10` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_11`;
CREATE TABLE `fishroleonlinelog_2016_5_11` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_12`;
CREATE TABLE `fishroleonlinelog_2016_5_12` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_13
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_13`;
CREATE TABLE `fishroleonlinelog_2016_5_13` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_14
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_14`;
CREATE TABLE `fishroleonlinelog_2016_5_14` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_15`;
CREATE TABLE `fishroleonlinelog_2016_5_15` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_16`;
CREATE TABLE `fishroleonlinelog_2016_5_16` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_17`;
CREATE TABLE `fishroleonlinelog_2016_5_17` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_18`;
CREATE TABLE `fishroleonlinelog_2016_5_18` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_19`;
CREATE TABLE `fishroleonlinelog_2016_5_19` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_2`;
CREATE TABLE `fishroleonlinelog_2016_5_2` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_20`;
CREATE TABLE `fishroleonlinelog_2016_5_20` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_21`;
CREATE TABLE `fishroleonlinelog_2016_5_21` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_22`;
CREATE TABLE `fishroleonlinelog_2016_5_22` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_23`;
CREATE TABLE `fishroleonlinelog_2016_5_23` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_24`;
CREATE TABLE `fishroleonlinelog_2016_5_24` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_25`;
CREATE TABLE `fishroleonlinelog_2016_5_25` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_26`;
CREATE TABLE `fishroleonlinelog_2016_5_26` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_27`;
CREATE TABLE `fishroleonlinelog_2016_5_27` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_28`;
CREATE TABLE `fishroleonlinelog_2016_5_28` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_29`;
CREATE TABLE `fishroleonlinelog_2016_5_29` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_3
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_3`;
CREATE TABLE `fishroleonlinelog_2016_5_3` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_30`;
CREATE TABLE `fishroleonlinelog_2016_5_30` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_31`;
CREATE TABLE `fishroleonlinelog_2016_5_31` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_4`;
CREATE TABLE `fishroleonlinelog_2016_5_4` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_5`;
CREATE TABLE `fishroleonlinelog_2016_5_5` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_6`;
CREATE TABLE `fishroleonlinelog_2016_5_6` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_7`;
CREATE TABLE `fishroleonlinelog_2016_5_7` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_5_9
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_5_9`;
CREATE TABLE `fishroleonlinelog_2016_5_9` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroleonlinelog_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroleonlinelog_2016_6_1`;
CREATE TABLE `fishroleonlinelog_2016_6_1` (
  `UserID` int(10) unsigned NOT NULL,
  `IsOnline` bit(1) NOT NULL,
  `GlobelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `MadelSum` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrceySum` int(10) unsigned NOT NULL DEFAULT '0',
  `MacAddress` varchar(57) NOT NULL,
  `IpAddress` varchar(17) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog`;
CREATE TABLE `fishroletablelog` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_10
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_10`;
CREATE TABLE `fishroletablelog_2016_1_10` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_11`;
CREATE TABLE `fishroletablelog_2016_1_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_12`;
CREATE TABLE `fishroletablelog_2016_1_12` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_13
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_13`;
CREATE TABLE `fishroletablelog_2016_1_13` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_14
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_14`;
CREATE TABLE `fishroletablelog_2016_1_14` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_15`;
CREATE TABLE `fishroletablelog_2016_1_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_16`;
CREATE TABLE `fishroletablelog_2016_1_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_17`;
CREATE TABLE `fishroletablelog_2016_1_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_18`;
CREATE TABLE `fishroletablelog_2016_1_18` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_19`;
CREATE TABLE `fishroletablelog_2016_1_19` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_20`;
CREATE TABLE `fishroletablelog_2016_1_20` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_21`;
CREATE TABLE `fishroletablelog_2016_1_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_22`;
CREATE TABLE `fishroletablelog_2016_1_22` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_23`;
CREATE TABLE `fishroletablelog_2016_1_23` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_24`;
CREATE TABLE `fishroletablelog_2016_1_24` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_25`;
CREATE TABLE `fishroletablelog_2016_1_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_26`;
CREATE TABLE `fishroletablelog_2016_1_26` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_27`;
CREATE TABLE `fishroletablelog_2016_1_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_28
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_28`;
CREATE TABLE `fishroletablelog_2016_1_28` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_29`;
CREATE TABLE `fishroletablelog_2016_1_29` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_30
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_30`;
CREATE TABLE `fishroletablelog_2016_1_30` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_31
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_31`;
CREATE TABLE `fishroletablelog_2016_1_31` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_5`;
CREATE TABLE `fishroletablelog_2016_1_5` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_6`;
CREATE TABLE `fishroletablelog_2016_1_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_7`;
CREATE TABLE `fishroletablelog_2016_1_7` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_8
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_8`;
CREATE TABLE `fishroletablelog_2016_1_8` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_1_9
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_1_9`;
CREATE TABLE `fishroletablelog_2016_1_9` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_1`;
CREATE TABLE `fishroletablelog_2016_2_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_10
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_10`;
CREATE TABLE `fishroletablelog_2016_2_10` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_11`;
CREATE TABLE `fishroletablelog_2016_2_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_12`;
CREATE TABLE `fishroletablelog_2016_2_12` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_15`;
CREATE TABLE `fishroletablelog_2016_2_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_16`;
CREATE TABLE `fishroletablelog_2016_2_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_17`;
CREATE TABLE `fishroletablelog_2016_2_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_18`;
CREATE TABLE `fishroletablelog_2016_2_18` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_19`;
CREATE TABLE `fishroletablelog_2016_2_19` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_2`;
CREATE TABLE `fishroletablelog_2016_2_2` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_20`;
CREATE TABLE `fishroletablelog_2016_2_20` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_21`;
CREATE TABLE `fishroletablelog_2016_2_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_22`;
CREATE TABLE `fishroletablelog_2016_2_22` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_23`;
CREATE TABLE `fishroletablelog_2016_2_23` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_24`;
CREATE TABLE `fishroletablelog_2016_2_24` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_25`;
CREATE TABLE `fishroletablelog_2016_2_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_26`;
CREATE TABLE `fishroletablelog_2016_2_26` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_27`;
CREATE TABLE `fishroletablelog_2016_2_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_28
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_28`;
CREATE TABLE `fishroletablelog_2016_2_28` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_29`;
CREATE TABLE `fishroletablelog_2016_2_29` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_3
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_3`;
CREATE TABLE `fishroletablelog_2016_2_3` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_4`;
CREATE TABLE `fishroletablelog_2016_2_4` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_5`;
CREATE TABLE `fishroletablelog_2016_2_5` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_6`;
CREATE TABLE `fishroletablelog_2016_2_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_7`;
CREATE TABLE `fishroletablelog_2016_2_7` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_8
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_8`;
CREATE TABLE `fishroletablelog_2016_2_8` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_2_9
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_2_9`;
CREATE TABLE `fishroletablelog_2016_2_9` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_1`;
CREATE TABLE `fishroletablelog_2016_3_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_11`;
CREATE TABLE `fishroletablelog_2016_3_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_15`;
CREATE TABLE `fishroletablelog_2016_3_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_16`;
CREATE TABLE `fishroletablelog_2016_3_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_17`;
CREATE TABLE `fishroletablelog_2016_3_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_18`;
CREATE TABLE `fishroletablelog_2016_3_18` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_2`;
CREATE TABLE `fishroletablelog_2016_3_2` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_21`;
CREATE TABLE `fishroletablelog_2016_3_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_25`;
CREATE TABLE `fishroletablelog_2016_3_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_27`;
CREATE TABLE `fishroletablelog_2016_3_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_3
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_3`;
CREATE TABLE `fishroletablelog_2016_3_3` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_31
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_31`;
CREATE TABLE `fishroletablelog_2016_3_31` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_4`;
CREATE TABLE `fishroletablelog_2016_3_4` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_5`;
CREATE TABLE `fishroletablelog_2016_3_5` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_6`;
CREATE TABLE `fishroletablelog_2016_3_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_7`;
CREATE TABLE `fishroletablelog_2016_3_7` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_3_8
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_3_8`;
CREATE TABLE `fishroletablelog_2016_3_8` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_1`;
CREATE TABLE `fishroletablelog_2016_4_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_11`;
CREATE TABLE `fishroletablelog_2016_4_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_12`;
CREATE TABLE `fishroletablelog_2016_4_12` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_13
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_13`;
CREATE TABLE `fishroletablelog_2016_4_13` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_15`;
CREATE TABLE `fishroletablelog_2016_4_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_16`;
CREATE TABLE `fishroletablelog_2016_4_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_17`;
CREATE TABLE `fishroletablelog_2016_4_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_18`;
CREATE TABLE `fishroletablelog_2016_4_18` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_19`;
CREATE TABLE `fishroletablelog_2016_4_19` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_2`;
CREATE TABLE `fishroletablelog_2016_4_2` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_20`;
CREATE TABLE `fishroletablelog_2016_4_20` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_21`;
CREATE TABLE `fishroletablelog_2016_4_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_22`;
CREATE TABLE `fishroletablelog_2016_4_22` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_23`;
CREATE TABLE `fishroletablelog_2016_4_23` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_24`;
CREATE TABLE `fishroletablelog_2016_4_24` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_25`;
CREATE TABLE `fishroletablelog_2016_4_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_26`;
CREATE TABLE `fishroletablelog_2016_4_26` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_27`;
CREATE TABLE `fishroletablelog_2016_4_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_29`;
CREATE TABLE `fishroletablelog_2016_4_29` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_30
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_30`;
CREATE TABLE `fishroletablelog_2016_4_30` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_4`;
CREATE TABLE `fishroletablelog_2016_4_4` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_4_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_4_6`;
CREATE TABLE `fishroletablelog_2016_4_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_1`;
CREATE TABLE `fishroletablelog_2016_5_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_10
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_10`;
CREATE TABLE `fishroletablelog_2016_5_10` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_11
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_11`;
CREATE TABLE `fishroletablelog_2016_5_11` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_12
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_12`;
CREATE TABLE `fishroletablelog_2016_5_12` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_13
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_13`;
CREATE TABLE `fishroletablelog_2016_5_13` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_14
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_14`;
CREATE TABLE `fishroletablelog_2016_5_14` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_15
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_15`;
CREATE TABLE `fishroletablelog_2016_5_15` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_16
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_16`;
CREATE TABLE `fishroletablelog_2016_5_16` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_17
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_17`;
CREATE TABLE `fishroletablelog_2016_5_17` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_18
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_18`;
CREATE TABLE `fishroletablelog_2016_5_18` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_19
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_19`;
CREATE TABLE `fishroletablelog_2016_5_19` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_2
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_2`;
CREATE TABLE `fishroletablelog_2016_5_2` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_20
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_20`;
CREATE TABLE `fishroletablelog_2016_5_20` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_21
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_21`;
CREATE TABLE `fishroletablelog_2016_5_21` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_22
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_22`;
CREATE TABLE `fishroletablelog_2016_5_22` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_23
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_23`;
CREATE TABLE `fishroletablelog_2016_5_23` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_24
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_24`;
CREATE TABLE `fishroletablelog_2016_5_24` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_25
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_25`;
CREATE TABLE `fishroletablelog_2016_5_25` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_26
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_26`;
CREATE TABLE `fishroletablelog_2016_5_26` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_27
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_27`;
CREATE TABLE `fishroletablelog_2016_5_27` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_28
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_28`;
CREATE TABLE `fishroletablelog_2016_5_28` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_29
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_29`;
CREATE TABLE `fishroletablelog_2016_5_29` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_3
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_3`;
CREATE TABLE `fishroletablelog_2016_5_3` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_30
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_30`;
CREATE TABLE `fishroletablelog_2016_5_30` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_31
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_31`;
CREATE TABLE `fishroletablelog_2016_5_31` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_4
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_4`;
CREATE TABLE `fishroletablelog_2016_5_4` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_5
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_5`;
CREATE TABLE `fishroletablelog_2016_5_5` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_6
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_6`;
CREATE TABLE `fishroletablelog_2016_5_6` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_7
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_7`;
CREATE TABLE `fishroletablelog_2016_5_7` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_8
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_8`;
CREATE TABLE `fishroletablelog_2016_5_8` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_5_9
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_5_9`;
CREATE TABLE `fishroletablelog_2016_5_9` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishroletablelog_2016_6_1
-- ----------------------------
DROP TABLE IF EXISTS `fishroletablelog_2016_6_1`;
CREATE TABLE `fishroletablelog_2016_6_1` (
  `UserID` int(10) unsigned NOT NULL,
  `TableTypeID` tinyint(3) unsigned NOT NULL,
  `TableMonthID` tinyint(3) unsigned NOT NULL,
  `GlobelNum` int(10) unsigned NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `CurrceyNum` int(10) unsigned NOT NULL,
  `JoinOrLeave` bit(1) NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for totalfishentityitemfinishlog
-- ----------------------------
DROP TABLE IF EXISTS `totalfishentityitemfinishlog`;
CREATE TABLE `totalfishentityitemfinishlog` (
  `ID` int(10) unsigned NOT NULL,
  `OrderID` varchar(64) NOT NULL,
  `HandleTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for totalfishentityitemlog
-- ----------------------------
DROP TABLE IF EXISTS `totalfishentityitemlog`;
CREATE TABLE `totalfishentityitemlog` (
  `ID` int(10) unsigned NOT NULL,
  `UserID` int(11) unsigned NOT NULL,
  `ItemID` int(11) unsigned NOT NULL,
  `ItemSum` smallint(6) unsigned NOT NULL,
  `Address` varchar(129) NOT NULL,
  `Phone` varchar(33) NOT NULL,
  `Name` varchar(11) NOT NULL,
  `ShopTime` datetime NOT NULL,
  `MedalNum` int(10) unsigned NOT NULL,
  `NowMedalNum` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for totalfishphonepaylog
-- ----------------------------
DROP TABLE IF EXISTS `totalfishphonepaylog`;
CREATE TABLE `totalfishphonepaylog` (
  `UserID` int(10) unsigned NOT NULL,
  `PhoneNumber` bigint(20) unsigned NOT NULL,
  `PhoneMoney` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for totalfishrechargelog
-- ----------------------------
DROP TABLE IF EXISTS `totalfishrechargelog`;
CREATE TABLE `totalfishrechargelog` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `UserID` int(10) unsigned NOT NULL,
  `UseRMBSum` int(10) unsigned NOT NULL,
  `orderid` varchar(256) NOT NULL,
  `channelOrderid` varchar(256) NOT NULL,
  `channelLabel` varchar(256) NOT NULL,
  `ChannelCode` varchar(256) NOT NULL,
  `AddGlobel` int(10) unsigned NOT NULL,
  `AddCurrceySum` int(10) unsigned NOT NULL,
  `LogTime` datetime NOT NULL,
  `OldGlobel` int(10) unsigned NOT NULL DEFAULT '0',
  `OldCurrcey` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Procedure structure for FishAddAnnouncementInfoLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddAnnouncementInfoLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddAnnouncementInfoLog`(IN `NickName` varchar(11),IN `ShopID` tinyint unsigned,IN `ShopOnlyID` tinyint unsigned,IN `NowTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('fishannouncementinfoLog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like fishannouncementinfoLog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(NickName,ShopID,ShopOnlyID,LogTime) values('
					,"'",NickName,"'",',',"'",ShopID,"'",',',"'",ShopOnlyID,"'",',',"'",NowTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddEntityItemLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddEntityItemLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddEntityItemLog`(IN `ID` int unsigned,IN `OrderStates` varchar(256),IN `ItemID` int unsigned,IN `ItemSum` int unsigned,IN `ShopLogTime` datetime,IN `HandleTime` datetime,IN `UserID` int unsigned,IN `Address` varchar(256),IN `Phone` bigint unsigned,IN `IDEntity` varchar(256),IN `Name` varchar(256),IN `OrderNumber` varchar(256),IN `MedalNum` int unsigned,IN `NowMedalNum` int unsigned,IN `HandleIP` varchar(16))
BEGIN
	insert into fishentityitemlog(ID,OrderStates,ItemID,ItemSum,ShopLogTime,HandleTime,UserID,Address,Phone,IDEntity,Name,OrderNumber,MedalNum,NowMedalNum,HandleIP)
	values(ID,OrderStates,ItemID,ItemSum,ShopLogTime,HandleTime,UserID,Address,Phone,IDEntity,Name,OrderNumber,MedalNum,NowMedalNum,HandleIP);
	select ROW_COUNT();
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddLogCarTableInfo
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddLogCarTableInfo`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddLogCarTableInfo`(IN `BrandUserID` int unsigned,IN `AreaGlobel` varchar(1024),IN `ResultIndex` smallint unsigned,IN `BrandGlobel` bigint,IN `SystemGlobel` bigint,IN `RoleSum` int unsigned,IN `LogTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('fishcartableinfo','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like fishcartableinfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(BrandUserID,AreaGlobel,ResultIndex,BrandGlobel,SystemGlobel,RoleSum,LogTime) values('
					,"'",BrandUserID,"'",',',"'",AreaGlobel,"'",',',"'",ResultIndex,"'",',',"'",BrandGlobel,"'",',',"'",SystemGlobel,"'",',',"'",RoleSum,"'",',',"'",LogTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddLogDialTableInfo
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddLogDialTableInfo`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddLogDialTableInfo`(IN `BrandUserID` int unsigned,IN `AreaData` varchar(256),IN `AreaGlobel` varchar(1024),IN `ResultIndex` smallint unsigned,IN `BrandGlobel` bigint,IN `SystemGlobel` bigint,IN `RoleSum` int unsigned,IN `LogTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('fishdialtableinfo','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like fishdialtableinfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(BrandUserID,AreaData,AreaGlobel,ResultIndex,BrandGlobel,SystemGlobel,RoleSum,LogTime) values('
					,"'",BrandUserID,"'",',',"'",AreaData,"'",',',"'",AreaGlobel,"'",',',"'",ResultIndex,"'",',',"'",BrandGlobel,"'",',',"'",SystemGlobel,"'",',',"'",RoleSum,"'",',',"'",LogTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddLogInfo
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddLogInfo`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddLogInfo`(IN `UserID` int unsigned,IN `TypeID` int unsigned,IN `TypeSum` int,IN `Param` int unsigned,IN `Info` varchar(25),IN `LogTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('FishLog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like FishLog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(UserID,TypeID,TypeSum,Info,Param,LogTime) values('
					,"'",UserID,"'",',',"'",TypeID,"'",',',"'",TypeSum,"'",',',"'",Info,"'",',',"'",Param,"'",",","'",LogTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddLogMonthInfo
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddLogMonthInfo`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddLogMonthInfo`(IN `UserID` int unsigned,IN `MonthID` tinyint unsigned,IN `MonthIndex` int unsigned,IN `MonthScore` int unsigned,IN `MonthSkillSum` int unsigned,IN `MonthAddGlobelSum` tinyint unsigned,IN `MonthRewardID` smallint unsigned,IN `LogTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('FishLogMonthInfo','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like FishLogMonthInfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(UserID,MonthID,MonthIndex,MonthScore,MonthSkillSum,MonthAddGlobelSum,MonthRewardID,LogTime) values('
					,"'",UserID,"'",',',"'",MonthID,"'",',',"'",MonthIndex,"'",',',"'",MonthScore,"'",',',"'",MonthSkillSum,"'",',',"'",MonthAddGlobelSum,"'",',',"'",MonthRewardID,"'",",","'",LogTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddLogNiuNiuTableInfo
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddLogNiuNiuTableInfo`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddLogNiuNiuTableInfo`(IN `BrandArray1` varchar(32),IN `BrandArray2` varchar(32),IN `BrandArray3` varchar(32),IN `BrandArray4` varchar(32),IN `BrandArray5` varchar(32),IN `UserSum` int unsigned,IN `ChangeGlobelSum` bigint,IN `SystemGlobelSum` bigint,IN `LogTime` datetime)
BEGIN

	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('fishniuniutableinfo','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like fishniuniutableinfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(BrandArray1,BrandArray2,BrandArray3,BrandArray4,BrandArray5,ChangeGlobelSum,SystemGlobelSum,LogTime,UserSum) values('
					,"'",BrandArray1,"'",',',"'",BrandArray2,"'",',',"'",BrandArray3,"'",',',"'",BrandArray4,"'",',',"'",BrandArray5,"'",',',"'",ChangeGlobelSum,"'",',',"'",SystemGlobelSum,"'",',',"'",LogTime,"'",',',"'",UserSum,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddLotteryInfo
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddLotteryInfo`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddLotteryInfo`(IN `UserID` int unsigned,IN `LotteryID` tinyint unsigned,IN `RewardID` smallint unsigned,IN `LogTime` datetime)
BEGIN
	insert into fishlotteryinfo(UserID,LotteryID,RewardID,LogTime)
	values (UserID,LotteryID,RewardID,LogTime);
	select ROW_COUNT();
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddPhonePayLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddPhonePayLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddPhonePayLog`(IN `OrderID` bigint unsigned,IN `FacePrice` int unsigned,IN `UserID` int unsigned,IN `PhoneNumber` bigint unsigned,IN `OrderStates` varchar(256),IN `LogTime` datetime)
BEGIN
	set @LogOrderID = 0;
	select OrderID into @LogOrderID from FishPhonePayLog where FishPhonePayLog.OrderID = OrderID;
	if @LogOrderID = 0 then
			insert into FishPhonePayLog(OrderID,UserID,FacePrice,PhoneNumber,OrderStates,LogTime)
			values(OrderID,UserID,FacePrice,PhoneNumber,OrderStates,LogTime);
	else
			update FishPhonePayLog set FishPhonePayLog.OrderStates = OrderStates,FishPhonePayLog.UserID= UserID,
									FishPhonePayLog.PhoneNumber = PhoneNumber,FishPhonePayLog.LogTime = LogTime where FishPhonePayLog.OrderID = OrderID;
	end if;
	SELECT ROW_COUNT();
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddRechargeInfoLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddRechargeInfoLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddRechargeInfoLog`(IN `UserID` int unsigned,IN `channelLabel` varchar(256),IN `OrderID` varchar(256),IN `channelorderid` varchar(256),IN `UseRMBSum` int unsigned,IN `AddGlobel` int unsigned,IN `AddCurrcey` int unsigned,IN `AddOrderUserName` varchar(16),IN `AddOrderUserIP` varchar(16),IN `AddOrderLogTime` datetime)
BEGIN
	insert into FishAddRechargeLog(UserID,channelLabel,orderid,channelOrderid,UseRMBSum,AddGlobel,AddCurrcey,AddOrderUserName,AddOrderUserIP,AddOrderLogTime)
		values(UserID,channelLabel,OrderID,channelorderid,UseRMBSum,AddGlobel,AddCurrcey,AddOrderUserName,AddOrderUserIP,AddOrderLogTime);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddRechargeLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddRechargeLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddRechargeLog`(IN `OrderStates` varchar(256),IN `OrderID` varchar(256),IN `UserID`  int unsigned,IN `ChannelCode` varchar(256),IN `ChannelOrderID` varchar(256),IN `ChannelLabel` varchar(256),IN `ShopItemID` int unsigned,IN `Price` int unsigned,IN `FreePrice` int unsigned,IN `OldGlobelNum` int unsigned,IN `OldCurrceyNum` int unsigned,IN `AddGlobelNum` int unsigned,IN `AddCurrceyNum` int unsigned,IN `LogTime` datetime,IN `AddRewardID` int unsigned)
BEGIN
  insert into FishRechargeLog(OrderStates,OrderID,UserID,ChannelCode,channelOrderid,channelLabel,ShopItemID,Price,FreePrice,OldGlobelNum,OldCurrceyNum,AddGlobelSum,AddCurrceySum,LogTime,AddRewardID)
		                   values(OrderStates,OrderID,UserID,ChannelCode,ChannelOrderID,ChannelLabel,ShopItemID,Price,FreePrice,OldGlobelNum,OldCurrceyNum,AddGlobelNum,AddCurrceyNum,LogTime,AddRewardID);
	Select ROW_COUNT();
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for Fishaddroleentityitemfinishlog
-- ----------------------------
DROP PROCEDURE IF EXISTS `Fishaddroleentityitemfinishlog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `Fishaddroleentityitemfinishlog`(IN `UserID` int unsigned,IN `ItemID` int unsigned,IN `ItemSum` smallint unsigned,IN `Address` varchar(128),IN `Phone` varchar(32),IN `Name` varchar(11),IN `NowTime` datetime,IN `OrderID` varchar(64),IN `HandleTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('Fishentityitemfinishlog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like Fishentityitemfinishlog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	insert into totalfishentityitemfinishlog(UserID,OrderID,HandleTime)
		values(UserID,OrderID,HandleTime);

	set @InsertStr = concat('insert into ',@LogName,'(UserID,OrderID,HandleTime) values('
					,"'",UserID,"'",',',"'",OrderID,"'",',',',',"'",HandleTime,"'",',',');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddRoleEntityItemLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddRoleEntityItemLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddRoleEntityItemLog`(IN `UserID` int unsigned,IN `ItemID` int unsigned,IN `ItemSum` smallint unsigned,IN `Address` varchar(128),IN `Phone` varchar(32),IN `Name` varchar(11),IN `NowTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('FishEntityItemLog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like FishEntityItemLog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(UserID,ItemID,ItemSum,Address,Phone,Name,ShopTime) values('
					,"'",UserID,"'",',',"'",ItemID,"'",',',"'",ItemSum,"'",',',"'",Address,"'",',',"'",Phone,"'",',',"'",Name,"'",',',"'",NowTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddRoleOnlineLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddRoleOnlineLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddRoleOnlineLog`(IN `UserID` int unsigned,IN `IsOnline` bit,IN `GlobelSum` int unsigned,IN `MedalSum` int unsigned,IN `CurrceySum` int unsigned,IN `MacAddress` varchar(57),IN `IPAddress` varchar(17),IN `LogTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('Fishroleonlinelog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like Fishroleonlinelog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(UserID,IsOnline,GlobelSum,MadelSum,CurrceySum,MacAddress,IpAddress,LogTime) values('
					,"'",UserID,"'",',',"'",IsOnline,"'",',',"'",GlobelSum,"'",',',"'",MedalSum,"'",',',"'",CurrceySum,"'",',',"'",MacAddress,"'",',',"'",IPAddress,"'",',',"'",LogTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddRoleTableLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddRoleTableLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddRoleTableLog`(IN `UserID` int unsigned,IN `TableTypeID` tinyint unsigned,IN `TableMonthID` tinyint unsigned,IN `GlobelNum` int unsigned,IN `MedalNum` int unsigned,IN `CurrceyNum` int unsigned,IN `JoinOrLeave` bit,IN `LogTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('fishroletablelog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like fishroletablelog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(UserID,TableTypeID,TableMonthID,GlobelNum,MedalNum,CurrceyNum,JoinOrLeave,LogTime) values('
					,"'",UserID,"'",',',"'",TableTypeID,"'",',',"'",TableMonthID,"'",',',"'",GlobelNum,"'",',',"'",MedalNum,"'",',',"'",CurrceyNum,"'",',',"'",JoinOrLeave,"'",',',"'",LogTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishAddUserMailLog
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishAddUserMailLog`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishAddUserMailLog`(IN `SrcUserID` int unsigned,IN `DestUserID` int unsigned,IN `Context` varchar(64),IN `RewardID`  smallint unsigned,IN `NowTime` datetime)
BEGIN
	#需要进行过滤 玩家发送最大的邮件数量
	-- 获取邮件的时间
  set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('FishMailLog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like FishMailLog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(SrcUserID,DestUserID,Context,RewardID,IsRead,IsExistsReward,SendTime) values('
					,"'",SrcUserID,"'",',',"'",DestUserID,"'",',',"'",Context,"'",',',"'",RewardID,"'",',',0,',',(RewardID!=0),',',"'",NowTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishCreateLogTable
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishCreateLogTable`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishCreateLogTable`(IN `NowYear` int unsigned,IN `NowMonth` int unsigned,IN `NowDay` int unsigned)
BEGIN
	-- 每天创建新的表
	
	set @LogName = CONCAT('FishLog','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = concat('create table if not exists ',@LogName,' like FishLog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @LogName = CONCAT('fishroletablelog','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = concat('create table if not exists ',@LogName,' like fishroletablelog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;


	set @LogName = CONCAT('Fishroleonlinelog','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = concat('create table if not exists ',@LogName,' like Fishroleonlinelog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @LogName = CONCAT('fishniuniutableinfo','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = concat('create table if not exists ',@LogName,' like fishniuniutableinfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @LogName = CONCAT('FishExChangeLog','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = concat('create table if not exists ',@LogName,' like FishExChangeLog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @LogName = CONCAT('FishLogMonthInfo','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = CONCAT('create table if not exists ',@LogName,' like FishLogMonthInfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @LogName = CONCAT('fishdialtableinfo','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = CONCAT('create table if not exists ',@LogName,' like fishdialtableinfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @LogName = CONCAT('fishcartableinfo','_',NowYear,'_',NowMonth,'_',NowDay);
	set @SqlStr = CONCAT('create table if not exists ',@LogName,' like fishcartableinfo;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	select 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for FishLogExChangeInfo
-- ----------------------------
DROP PROCEDURE IF EXISTS `FishLogExChangeInfo`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FishLogExChangeInfo`(IN `UserID` int unsigned,IN `TypeID` tinyint unsigned,IN `ExChangeCode` varchar(17),IN `LogTime` datetime)
BEGIN
	set @NowYear = year(now());
	set @NowMonth = month(now());
	set @NowDay = day(now());
	set @LogName = CONCAT('FishExChangeLog','_',@NowYear,'_',@NowMonth,'_',@NowDay);

	set @SqlStr = concat('create table if not exists ',@LogName,' like FishExChangeLog;');
	prepare stmt from @SqlStr;
	EXECUTE stmt;

	set @InsertStr = concat('insert into ',@LogName,'(UserID,TypeID,ExChange,LogTime) values('
					,"'",UserID,"'",',',"'",TypeID,"'",',',"'",ExChangeCode,"'",',',"'",LogTime,"'",');');
	
	prepare stmt2 from @InsertStr;
	EXECUTE stmt2;

	select 1;

END
;;
DELIMITER ;
