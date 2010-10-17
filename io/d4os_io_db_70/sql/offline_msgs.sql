CREATE TABLE IF NOT EXISTS `offline_msgs` (
  `uuid` varchar(36) NOT NULL,
  `message` text NOT NULL,
  KEY `uuid` (`uuid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
