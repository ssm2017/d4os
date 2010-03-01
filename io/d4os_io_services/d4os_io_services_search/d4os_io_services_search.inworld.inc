<?php
/**
* @package		d4os
* @copyright	Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license		GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

function d4os_io_services_search_registerhost() {
  $hostname = $_GET['host'];
  $port     = $_GET['port'];
  $service  = $_GET['service'];
  db_set_active('os_search');
  if ($hostname != "" && $port != "" && $service == "online") {
    // Check if there is already a database row for this host
    $checkhost = db_result(db_query("SELECT COUNT(register) FROM {hostsregister} WHERE host = '%s' AND port = '%s'", array($hostname, $port)));

    // Get the request time as a timestamp for later
    $timestamp = $_SERVER['REQUEST_TIME'];
    
    // if greater then 1, check the lastcheck date
    if ($checkhost > 0) {
      db_query("UPDATE {hostsregister} SET register = '%s' WHERE host = '%s' AND port = '%s'", array($timestamp, $hostname, $port));
    }
    else {
      db_query("INSERT INTO {hostsregister} VALUES ('%s', '%s', '%s', 0, 0)", array($hostname, $port, $timestamp));
    }
  }
  elseif ($hostname != "" && $port != "" && $service = "offline") {
    db_query("DELETE FROM {hostsregister} WHERE host = '%s' AND port = '%s'", array($hostname, $port));
  }
  db_set_active('default');
}