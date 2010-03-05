<?php
/**
* @package    d4os
* @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

function d4os_io_db_os_search_services_parser_run() {
  watchdog('d4os_io_services_search_parser', t('Starting parsing host values.'), array(), WATCHDOG_INFO);
  global $now;
  // get time
  $now = time();
  // get the hosts infos
  d4os_io_db_set_active('os_search');
  $jobsearch = db_query("SELECT host, port FROM {hostsregister} WHERE lastcheck < %d LIMIT 0,1", $now);
  $hosts = array();
  while ($host = db_fetch_object($jobsearch)) {
    $hosts[] = $host;
  }
  d4os_io_db_set_active('default');
  if (count($hosts) > 0) {
    foreach($hosts as $host) {
      d4os_io_db_os_search_services_parser_checkhost($host->host, $host->port);
      watchdog('d4os_io_services_search_parser', t('Host !host was parsed.'), array('!host' => $host->host), WATCHDOG_INFO);
    }
  }
  else {
    watchdog('d4os_io_services_search_parser', t('No host found.'), array(), WATCHDOG_WARNING);
  }
}

function d4os_io_db_os_search_services_parser_checkhost($hostname, $port) {
  // call the host
  $response = drupal_http_request('http://'. $hostname. ':'. $port);
  if ($response->code != 404) {
    watchdog('d4os_io_services_search_parser', t('Host !host was unreachable.'), array('!host' => $hostname), WATCHDOG_ERROR);
    d4os_io_db_set_active('os_search');
    $check = db_query("UPDATE {hostsregister} SET failcounter = failcounter + 1 WHERE host = '%s' AND port = '%s'", array($hostname, $port));

    //Setting a "fake" update time so this host will have time
    //to get back online
    //$next = time() + 600; // 10 mins, so we don't get stuck
    //
    $updater = db_query("UPDATE {hostsregister} SET lastcheck = %d WHERE host = '%s' AND port = '%s'", array($next, $hostname, $port));
    d4os_io_db_set_active('default');
  }
  else {
    d4os_io_db_set_active('os_search');
    $sql = db_query("UPDATE {hostsregister} set failcounter = 0 WHERE host = '%s' AND port = '%s'", array($hostname, $port));
    d4os_io_db_set_active('default');
    d4os_io_db_os_search_services_parser_parse($hostname, $port);
  }
}

function d4os_io_db_os_search_services_parser_parse($hostname, $port) {
  global $now;
  if ($hostname != "" && $port != "") {
    $next = time() + (variable_get('d4os_io_db_os_search_services_parser_timer', 15) * 60);
    d4os_io_db_set_active('os_search');
    $updater = db_query("UPDATE hostsregister set lastcheck = %d WHERE host = '%s' AND port = '%s'", array($next, $hostname, $port));
    d4os_io_db_set_active('default');

    // Load XML doc from URL
    $objDOM = new DOMDocument();
    $objDOM->resolveExternals = false;
    $response = drupal_http_request('http://'. $hostname. ':'. $port. '?method=collector');
    if ($response->code == 200) {
      $objDOM->loadXML($response->data);

      // Grabbing the expire to update
      $regiondata = $objDOM->getElementsByTagName("regiondata")->item(0);
      $expire = $regiondata->getElementsByTagName("expire")->item(0)->nodeValue;

      // Calculate new expire
      $next = time() + $expire;

      d4os_io_db_set_active('os_search');
      $updater = db_query("UPDATE {hostsregister} SET lastcheck = %d WHERE host = '%s' AND port = '%s'", array($next, $hostname, $port));
      d4os_io_db_set_active('default');
    
      $regionlist = $regiondata->getElementsByTagName("region");
    
      foreach ($regionlist as $region ) {

        // Start reading the Region info
        $info         = $region->getElementsByTagName("info")->item(0);
        $regionuuid   = $info->getElementsByTagName("uuid")->item(0)->nodeValue;
        $regionname   = $info->getElementsByTagName("name")->item(0)->nodeValue;
        $regionhandle = $info->getElementsByTagName("handle")->item(0)->nodeValue;
        $url          = $info->getElementsByTagName("url")->item(0)->nodeValue;

        // First, check if we already have a region that is the same
        d4os_io_db_set_active('os_search');
        $check = db_query("SELECT count(*) FROM {regions} WHERE regionuuid = '%s'", $regionuuid);

        if (db_result($check) > 0) {
          db_query("DELETE FROM {regions} WHERE regionuuid = '%s'", $regionuuid);
          db_query("DELETE FROM {parcels} WHERE regionuuid = '%s'", $regionuuid);
          db_query("DELETE FROM {objects} WHERE regionuuid = '%s'", $regionuuid);
          db_query("DELETE FROM {allparcels} WHERE regionUUID = '%s'", $regionuuid);
          db_query("DELETE FROM {parcelsales} WHERE regionUUID = '%s'", $regionuuid);
        }
        d4os_io_db_set_active('default');

        $data     = $region->getElementsByTagName("data")->item(0);
        $estate   = $data->getElementsByTagName("estate")->item(0);
        $username = $estate->getElementsByTagName("name")->item(0)->nodeValue;
        $useruuid = $estate->getElementsByTagName("uuid")->item(0)->nodeValue;

        // Second, add the new info to the database
        d4os_io_db_set_active('os_search');
        db_query("INSERT INTO {regions} VALUES('%s','%s','%s','%s','%s','%s')", array($regionname, $regionuuid, $regionhandle, $url, $username, $useruuid));
        d4os_io_db_set_active('default');

        // Start reading the parcel info
        $parcel = $data->getElementsByTagName("parcel");
        
        foreach ($parcel as $value ) {
          $parcelname         = $value->getElementsByTagName("name")->item(0)->nodeValue;
          $parceluuid         = $value->getElementsByTagName("uuid")->item(0)->nodeValue;
          $infouuid           = $value->getElementsByTagName("infouuid")->item(0)->nodeValue;
          $parcellanding      = $value->getElementsByTagName("location")->item(0)->nodeValue;
          $parceldescription  = $value->getElementsByTagName("description")->item(0)->nodeValue;
          $parcelarea         = $value->getElementsByTagName("area")->item(0)->nodeValue;
          $parcelcategory     = $value->getAttributeNode("category")->nodeValue;
          $parcelsaleprice    = $value->getAttributeNode("salesprice")->nodeValue;
          $dwell              = $value->getElementsByTagName("dwell")->item(0)->nodeValue;
          $owner              = $value->getElementsByTagName("owner")->item(0);
          $owneruuid          = $owner->getElementsByTagName("uuid")->item(0)->nodeValue;

          // Adding support for groups
          $group = $value->getElementsByTagName("group")->item(0);

          if ($group != "") {
            $groupuuid = $group->getElementsByTagName("groupuuid")->item(0)->nodeValue;
          }
          else {
            $groupuuid = "00000000-0000-0000-0000-000000000000";
          }
    
          // Check bits on Public, Build, Script
          $parcelforsale    = $value->getAttributeNode("forsale")->nodeValue;
          $parceldirectory  = $value->getAttributeNode("showinsearch")->nodeValue;
          $parcelbuild      = $value->getAttributeNode("build")->nodeValue;
          $parcelscript     = $value->getAttributeNode("scripts")->nodeValue;
          $parcelpublic     = $value->getAttributeNode("public")->nodeValue;
    
          // Save
          $values = array(
            $regionuuid,
            $parcelname,
            $owneruuid,
            $groupuuid,
            $parcellanding,
            $parceluuid,
            $infouuid,
            $parcelarea
          );
          d4os_io_db_set_active('os_search');
          db_query("INSERT INTO {allparcels} VALUES('%s','%s','%s','%s','%s','%s','%s','%s' )", $values);
          d4os_io_db_set_active('default');
      
          if ($parceldirectory == "true") {
            $values = array(
              $regionuuid,
              $parcelname,
              $parceluuid,
              $parcellanding,
              $parceldescription,
              $parcelcategory,
              $parcelbuild,
              $parcelscript,
              $parcelpublic,
              $dwell,
              $infouuid
            );
            d4os_io_db_set_active('os_search');
            db_query("INSERT INTO {parcels} VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s' )", $values);
            d4os_io_db_set_active('default');
          }
      
          if ($parcelforsale == "true") {
            $values = array(
              $regionuuid,
              $parcelname,
              $parceluuid,
              $parcelarea,
              $parcelsaleprice,
              $parcellanding,
              $infouuid,
              $dwell,
              "1",
              "false"
            );
            d4os_io_db_set_active('os_search');
            db_query("INSERT INTO {parcelsales} VALUES('%s','%s','%s','%s','%s','%s','%s', '%s', '%s', '%s')", $values);
            d4os_io_db_set_active('default');
          }
        }
    
        // Handle objects
        $objects = $data->getElementsByTagName("object");
    
        foreach($objects as $value ) {
          $uuid         = $value->getElementsByTagName("uuid")->item(0)->nodeValue;
          $regionuuid   = $value->getElementsByTagName("regionuuid")->item(0)->nodeValue;
          $parceluuid   = $value->getElementsByTagName("parceluuid")->item(0)->nodeValue;
          $title        = $value->getElementsByTagName("title")->item(0)->nodeValue;
          $description  = $value->getElementsByTagName("description")->item(0)->nodeValue;
          $flags        = $value->getElementsByTagName("flags")->item(0)->nodeValue;
      
          $values = array(
            $uuid,
            $parceluuid,
            $title,
            $description,
            $regionuuid
          );
          d4os_io_db_set_active('os_search');
          db_query("INSERT INTO {objects} VALUES('%s','%s','','%s','%s','%s')", $values);
          d4os_io_db_set_active('default');
        }
      }
    }
    return True;
  }
  else {
    watchdog('d4os_io_services_search_parser', t('XML data for !host was unreachable.'), array('!host' => $jobs[0]), WATCHDOG_ERROR);
    return False;
  }
}
