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

function d4os_io_db_os_search_user_delete($uuid) {
  // get the uid
  $uid = d4os_io_db_os_user_get_uid($uuid);

  d4os_io_db_set_active('os_search');
  // delete parcels
  db_query("DELETE FROM {allparcels} WHERE ownerUUID = '%s'", array($uuid));
  // delete classifieds
  db_query("DELETE FROM {classifieds} WHERE creatoruuid = '%s'", array($uuid));
  d4os_io_db_set_active('default');

  // delete events
  $result = db_query("SELECT nid FROM {node} WHERE uid='%s' AND type='grid_event'", array($uid));
  $events = array();
  while ($event = db_fetch_object($result)) {
    $events[] = $event->nid;
  }
  foreach ($events as $event) {
    node_delete($event->nid);
  }
}

function d4os_io_db_os_event_save($event) {
// TODO : check if success before saving the node itself
  // convert array to object
  $event = is_array($event) ? (object)$event : $event;
  // get inworld fields
  $grid_fields = d4os_ui_events_get_grid_fields();
  unset($grid_fields['eventid']);

  if (is_object($event) && $event->eventid != NULL) {
    $grid_query = '';
    $grid_v = array();
    foreach ($event as $key => $value) {
      if (in_array($key, $grid_fields)) {
        switch ($key) {
          case 'owneruuid':
          case 'creatoruuid':
          case 'simname':
          case 'globalPos':
          case 'mature':
            $grid_query .= "$key = '%s', ";
            $grid_v[]    = $value;
            break;
          case 'category':
          case 'dateUTC':
          case 'duration':
          case 'covercharge':
          case 'coveramount':
          case 'eventflags':
            $grid_query .= "$key = %d, ";
            $grid_v[]    = $value;
            break;
          case 'name':
          case 'description':
            $grid_query .= "$key = '%s', ";
            $grid_v[]    = $value;
            break;
        }
      }
    }

    // remove the last ", "
    $grid_query = substr($grid_query, 0, -2);
    // make the query
    d4os_io_db_set_active('os_search');
    $success = db_query("UPDATE {events} SET $grid_query WHERE `eventid` = '%d'", array_merge($grid_v, array($event->eventid)));
    d4os_io_db_set_active('default');
    if (!$success) {
      // The query failed - better to abort the save than risk further data loss.
      drupal_set_message(t('Error saving the event to the grid database'), 'error');
      return FALSE;
    }
  }
  else {
    $fields = array();
    $values = array();
    $s      = array();
    // create the new event
    foreach ($event as $key => $value) {
      if (in_array($key, $grid_fields)) {
        switch ($key) {
          case 'owneruuid':
          case 'creatoruuid':
          case 'simname':
          case 'globalPos':
          case 'mature':
            $fields[] = $key;
            $values[] = $value;
            $s[] = "'%s'";
            break;
          case 'category':
          case 'dateUTC':
          case 'duration':
          case 'covercharge':
          case 'coveramount':
          case 'eventflags':
            $fields[] = $key;
            $values[] = $value;
            $s[] = "%d";
            break;
          case 'name':
          case 'description':
            $fields[] = $key;
            $values[] = $value;
            $s[] = "'%s'";
            break;
        }
      }
    }

    $query = 'INSERT INTO {events} ('. implode(', ', $fields) .') VALUES ('. implode(', ', $s) .')';
    d4os_io_db_set_active('os_search');
    $success = db_query($query, $values);
    $eventid = db_last_insert_id('events', 'eventid');
    d4os_io_db_set_active('default');
    if (!$success) {
      // The query failed - better to abort the save than risk further data loss.
      drupal_set_message(t('Error saving the event to the grid database'), 'error');
      return FALSE;
    }
    // save the node item
    $values = array(
      $event->nid,
      $event->vid,
      $eventid
    );
    $success = db_query('INSERT INTO {d4os_ui_events} (nid, vid, eventid) VALUES (%d, %d, %d)', $values);
    if (!$success) {
      // The query failed - better to abort the save than risk further data loss.
      drupal_set_message(t('Error saving the event to the website database'), 'error');
      return FALSE;
    }
  }

  // Refresh event object.
  $event = d4os_io_db_os_event_load($event);

  return $event;
}


function d4os_io_db_os_event_load($data = array()) {
  if (is_numeric($data)) {
    // get event id from node
    $event_id = db_result(db_query("SELECT eventid FROM {d4os_ui_events} WHERE nid=%d", $data));
    if (!$event_id) {
      return FALSE;
    }
    // get event values
    $data = array('eventid' => $event_id);
  }
  // get inworld fields
  $fields = d4os_ui_events_get_grid_fields();

  // Dynamically compose a SQL query:
  $query = array();
  $params = array();

  if (!is_array($data)) {
    return FALSE;
  }

  foreach ($data as $key => $value) {
    if (in_array($key, $fields)) {
      switch ($key) {
        case 'owneruuid':
        //case 'name':
        case 'creatoruuid':
        //case 'description':
        case 'simname':
        case 'globalPos':
        case 'mature':
          $query[]= $key. " = '%s'";
          $params[] = $value;
          break;
        case 'category':
        case 'dateUTC':
        case 'duration':
        case 'covercharge':
        case 'coveramount':
        case 'eventflags':
        case 'eventid':
          $query[]= $key. " = %d";
          $params[] = $value;
          break;
      }
    }
  }
  $query = 'SELECT * FROM {events} WHERE '. implode(' AND ', $query);
  d4os_io_db_set_active('os_search');
  $result = db_query($query, $params);
  if ($event = db_fetch_object($result)) {
    d4os_io_db_set_active('default');
    // get nid
    $result = db_query("SELECT * FROM {d4os_ui_events} WHERE eventid=%d", $event->eventid);
    if ($website_event = db_fetch_object($result)) {
      $event->nid = $website_event->nid;
      $event->vid = $website_event->vid;
    }
    unset($event->name);
    return $event;
  }
  else {
    d4os_io_db_set_active('default');
    return FALSE;
  }
}

function d4os_io_db_os_get_nid_by_eventid($eventid) {
  return db_result(db_query("SELECT nid FROM {d4os_ui_events} WHERE eventid=%d", $eventid));
}

function d4os_io_db_os_event_load_all($time_limit = NULL, $order = NULL) {
  $events = array();
  $query = "SELECT * FROM {events}";
  if (!is_null($time_limit)) {
    $query .= " WHERE dateUTC >= ". $time_limit. " OR (dateUTC + duration) >= ". $time_limit;
  }
  if (!is_null($order)) {
    $query .= " ORDER BY ". $order;
  }
  d4os_io_db_set_active('os_search');
  $result = db_query($query);
  while ($event = db_fetch_object($result)) {
    $events[] = $event;
  }
  d4os_io_db_set_active('default');
  return $events;
}

function d4os_io_db_os_event_delete($nid) {
  // load the local event
  $eventid = db_result(db_query('SELECT eventid FROM {d4os_ui_events} WHERE nid=%d', $nid));
  d4os_io_db_set_active('os_search');
  db_query("DELETE FROM {events} WHERE eventid=%d", $eventid);
  d4os_io_db_set_active('default');
  db_query("DELETE FROM {d4os_ui_events} WHERE eventid=%d", $eventid);
}

function d4os_io_db_os_event_delete_revision($vid) {
  db_query('DELETE FROM {d4os_ui_events} WHERE vid = %d', $node->vid);
}

function d4os_io_db_os_events_get_parcels_by_owner($ownerid) {
  d4os_io_db_set_active('os_search');
  $result = db_query("SELECT regionUUID, parcelname, landingpoint FROM {allparcels} WHERE ownerUUID = '%s'", $ownerid);
  $parcels = array();
  while($parcel = db_fetch_array($result)) {
    $parcels[] = $parcel;
  }
  d4os_io_db_set_active('default');
  if (count($parcels)) {
    return $parcels;
  }
  return FALSE;
}



/*
 * Grid Monitor tools
 */
function d4os_io_db_os_search_hosts_count() {
  d4os_io_db_set_active('os_search');
  $count = db_result(db_query("SELECT COUNT(*) FROM {hostsregister}"));
  d4os_io_db_set_active('default');
  return $count;
}

function d4os_io_db_os_search_hosts_list_paged($page = 0, $limit = 5, $headers) {
  if (!isset($_GET['page'])) {
    $_GET['page'] = $page;
  }
  if (!isset($_GET['order'])) {
    $_GET['order'] = 'host';
  }
  if (!isset($_GET['sort'])) {
    $_GET['sort'] = 'DESC';
  }
  d4os_io_db_set_active('os_search');
  $result = pager_query('SELECT * FROM {hostsregister} %s', $limit, 0, NULL, array(tablesort_sql($headers)));
  while ($host = db_fetch_object($result)) {
    $items[] = $host;
  }
  d4os_io_db_set_active('default');
  return $items;
}

function d4os_io_db_os_search_hosts_delete($hostname, $hostport) {
  // get regions
  $regions = array();
  $url = 'http://'. $hostname. ':'. $hostport;
  d4os_io_db_set_active('os_search');
  $result = pager_query("SELECT * FROM {regions} WHERE url = '%s'", $url);
  while ($region = db_fetch_object($result)) {
    $regions[] = $region;
  }
  d4os_io_db_set_active('default');

  // delete regions, parcels, objects, etc...
  foreach ($regions as $region) {
    d4os_io_db_set_active('os_search');
    db_query("DELETE FROM {regions} WHERE regionuuid = '%s'", $region->regionuuid);
    db_query("DELETE FROM {parcels} WHERE regionuuid = '%s'", $region->regionuuid);
    db_query("DELETE FROM {objects} WHERE regionuuid = '%s'", $region->regionuuid);
    db_query("DELETE FROM {allparcels} WHERE regionUUID = '%s'", $region->regionuuid);
    db_query("DELETE FROM {parcelsales} WHERE regionUUID = '%s'", $region->regionuuid);
    d4os_io_db_set_active('default');
  }

  // delete host
  d4os_io_db_set_active('os_search');
  db_query("DELETE FROM {hostsregister} WHERE host = '%s' AND port = '%s'", array($hostname, $hostport));
  d4os_io_db_set_active('default');
  watchdog('d4os_io_services_search_parser', t('Host !host was deleted.'), array('!host' => $hostname. ':'. $hostport), WATCHDOG_WARNING);
}
/*
 * search services
 * The code in this script was inspired and mostly copied from ossearch
 * http://forge.opensimulator.org/gf/project/ossearch
 * Copyright (c)Melanie Thielker (http://opensimulator.org/)
 */

function d4os_io_db_os_search_services_dir_places_query($params) {
  $category = $params['category'];
  $text     = $params['text'];
  $start    = $params['start'];
  
  if ($category != -1) {
    $values = array($category, $text, $text, $start, 100);
    $query = "SELECT * FROM {parcels} WHERE ".
              "(searchcategory = -1 or searchcategory = '%s') ".
              "AND (parcelname LIKE '%%%s%%' ".
              "OR description LIKE '%%%s%%') ".
              "ORDER BY dwell DESC, parcelname LIMIT %d, %d";
  }
  else {
    $values = array($text, $text, $start, 100);
    $query = "SELECT * FROM {parcels} WHERE ".
              "parcelname LIKE '%%%s%%' ".
              "OR description LIKE '%%%s%%' ".
              "ORDER BY dwell DESC, parcelname LIMIT %d, %d";
  }
  $data = array();

  d4os_io_db_set_active('os_search');
  $result = db_query($query, $values);
  while ($row = db_fetch_array($result)) {
    $data[] = array(
      "parcel_id" => $row["infouuid"],
      "name"      => $row["parcelname"],
      "for_sale"  => "False",
      "auction"   => "False",
      "dwell"     => $row["dwell"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_db_os_search_services_dir_popular_query($params) {
  $flags = $params['flags'];

  $terms    = array();
  
  if ($flags & 0x1000) {
    $terms[] = "has_picture = 1";
  }

  if ($flags & 0x0800) {
    $terms[] = "mature = 0";
  }

  $where = "";
  if (count($terms) > 0) {
    $where = " WHERE " . join(" AND ", $terms);
  }
  
  d4os_io_db_set_active('os_search');
  $result = mysql_query("SELECT * FROM {popularplaces}" . $where);
  
  $data = array();
  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "parcel_id" => $row["infoUUID"],
      "name"      => $row["name"],
      "dwell"     => $row["dwell"]
    );
  }
  d4os_io_db_set_active('default');
  
  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_db_os_search_services_dir_land_query($params) {
  $flags       = $params['flags'];
  $type        = $params['type'];
  $price       = $params['price'];
  $area        = $params['area'];
  $query_start = $params['query_start'];

  $terms = array();
  $order = "lsq";

  if ($flags & 0x80000) {
    $order = "parcelname";
  }

  if ($flags & 0x10000) {
    $order = "saleprice";
  }

  if ($flags & 0x40000) {
    $order = "area";
  }

  if (!($flags & 0x8000)) {
    $order .= " desc";
  }

  if ($flags & 0x100000) {
    $terms[] = "saleprice <= '" . db_escape_string($price) . "'";
  }

  if ($flags & 0x200000) {
    $terms[] = "area >= '" . db_escape_string($area) . "'";
  }

  if (($type & 26) == 2) { // Auction
    return array(
      'success'      => FALSE,
      'errorMessage' => t('No auctions listed.'),
    );
  }

  if (($type & 24) == 8) {
    $terms[] = "parentestate = 1";
  }

  if (($type & 24) == 16) {
    $terms[] = "parentestate <> 1";
  }

  if ($flags & 0x800) {
    $terms[] = "mature = 'false'";
  }

  if ($flags & 0x4000) {
    $terms[] = "mature = 'true'";
  }

  $where = "";
  if (count($terms) > 0) {
    $where = " WHERE " . join(" AND ", $terms);
  }

  d4os_io_db_set_active('os_search');
  $sql = "SELECT *, saleprice/area AS lsq FROM {parcelsales}" . $where . " ORDER BY " . $order;
  $result = db_query_range($sql, $query_start, 101);
  
  $data = array();
  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "parcel_id"     => $row["infoUUID"],
      "name"          => $row["parcelname"],
      "auction"       => "false",
      "for_sale"      => "true",
      "sale_price"    => $row["saleprice"],
      "landing_point" => $row["landingpoint"],
      "region_UUID"   => $row["regionUUID"],
      "area"          => $row["area"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_db_os_search_services_dir_events_query($params) {
  $text         = $params['text'];
  $flags        = $params['flags'];
  $query_start  = $params['query_start'];

  $pieces = explode("|", $text);

  $day      = $pieces[0];
  $category = $pieces[1];

  //Setting a variable for NOW
  $now = time();

  $terms = array();

  if ($day == "u") $terms[] = "dateUTC > ".$now."";

  if ($category <> 0) $terms[] = "category = ".$category."";

  if ($flags & 0x2000) $terms[] = "mature = 'false'";

  $where = "";

  if (count($terms) > 0) {
    $where = " WHERE " . join(" AND ", $terms);
  }

  d4os_io_db_set_active('os_search');
  $result = db_query_range("SELECT * FROM {events}". $where, $query_start, 101);

  $data = array();

  while ($row = db_fetch_array($result)) {
    $date = strftime("%m/%d %I:%M %p",$row["dateUTC"]);
    $data[] = array(
      "owner_id"    => $row["owneruuid"],
      "name"        => $row["name"],
      "event_id"    => $row["eventid"],
      "date"        => $date,
      "unix_time"   => $row["dateUTC"],
      "event_flags" => $row["eventflags"]
    );
  }
  d4os_io_db_set_active('default');
  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_db_os_search_services_dir_classified_query($params) {
  $text         = $params['text'];
  $flags        = $params['flags'];
  $category     = $params['category'];
  $query_start  = $params['query_start'];

  if ($category <> 0) $terms[] = "category = ".$category."";

  $where = "";

  if (count($terms) > 0) {
    $where = " where " . join(" and ", $terms);
  }

  d4os_io_db_set_active('os_search');
  $result = db_query_range("SELECT * FROM {classifieds}". $where, $query_start, 101);

  $data = array();
  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "classifiedid"    => $row["classifieduuid"],
      "name"            => $row["name"],
      "classifiedflags" => $row["classifiedflags"],
      "creation_date"   => $row["creationdate"],
      "expiration_date" => $row["expirationdate"],
      "priceforlisting" => $row["priceforlisting"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_db_os_search_services_event_info_query($params) {
  $eventID = $params['eventID'];

  d4os_io_db_set_active('os_search');
  $result = db_query("SELECT * FROM {events} WHERE eventID = %d", $eventID);

  $data = array();
  while (($row = db_fetch_array($result))) {
    $date = strftime("%G-%m-%d %H:%M:%S",$row["dateUTC"]);

    if ($row['category'] == 18) $category = "Discussion";
    if ($row['category'] == 19) $category = "Sports";
    if ($row['category'] == 20) $category = "Live Music";
    if ($row['category'] == 22) $category = "Commercial";
    if ($row['category'] == 23) $category = "Nightlife/Entertainment";
    if ($row['category'] == 24) $category = "Games/Contests";
    if ($row['category'] == 25) $category = "Pageants";
    if ($row['category'] == 26) $category = "Education";
    if ($row['category'] == 27) $category = "Arts and Culture";
    if ($row['category'] == 28) $category = "Charity/Support Groups";
    if ($row['category'] == 29) $category = "Miscellaneous";

    $data[] = array(
      "event_id"        => $row["eventid"],
      "creator"         => $row["creatoruuid"],
      "name"            => $row["name"],
      "category"        => $category,
      "description"     => drupal_html_to_text($row["description"]),
      "date"            => $date,
      "dateUTC"         => $row["dateUTC"],
      "duration"        => $row["duration"],
      "covercharge"     => $row["covercharge"],
      "coveramount"     => $row["coveramount"],
      "simname"         => $row["simname"],
      "globalposition"  => $row["globalPos"],
      "eventflags"      => $row["eventflags"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_db_os_search_services_classifieds_info_query($params) {
  $classifiedID = $params['classifiedID'];

  d4os_io_db_set_active('os_search');
  $result = db_query("SELECT * FROM {classifieds} WHERE classifieduuid = '%s'", $classifiedID);

  $data = array();
  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "classifieduuid"  => $row["classifieduuid"],
      "creatoruuid"     => $row["creatoruuid"],
      "creationdate"    => $row["creationdate"],
      "expirationdate"  => $row["expirationdate"],
      "category"        => $row["category"],
      "name"            => $row["name"],
      "description"     => $row["description"],
      "parceluuid"      => $row["parceluuid"],
      "parentestate"    => $row["parentestate"],
      "snapshotuuid"    => $row["snapshotuuid"],
      "simname"         => $row["simname"],
      "posglobal"       => $row["posglobal"],
      "parcelname"      => $row["parcelname"],
      "classifiedflags" => $row["classifiedflags"],
      "priceforlisting" => $row["priceforlisting"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

/*
 * inworld requests
 */
function d4os_io_db_os_search_services_registerhost($params) {
  $hostname = $params['host'];
  $port     = $params['port'];
  $service  = $params['service'];
  d4os_io_db_set_active('os_search');
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
  d4os_io_db_set_active('default');
}

/*
 * search parser
 */
function d4os_io_db_os_search_services_parser_run() {
  watchdog('d4os_io_services_search_parser', t('Starting parsing host values.'), array(), WATCHDOG_INFO);
  global $now;
  // get time
  $now = time();
  // get the hosts infos
  $hosts = array();
  // get the hosts qty to parse in one cron run
  $qty = variable_get('d4os_io_services_search_parser_host_qty', 1);
  $limit = ($qty == 0) ? '' : ' LIMIT 0,'. $qty;
  d4os_io_db_set_active('os_search');
  $jobsearch = db_query("SELECT * FROM {hostsregister} WHERE lastcheck < %d". $limit, $now);
  while ($host = db_fetch_object($jobsearch)) {
    $hosts[] = $host;
  }
  d4os_io_db_set_active('default');
  if (count($hosts) > 0) {
    foreach($hosts as $host) {
      d4os_io_db_os_search_services_parser_checkhost($host);
      watchdog('d4os_io_services_search_parser', t('Host !host was parsed.'), array('!host' => $host->host. ':'. $host->port), WATCHDOG_INFO);
    }
  }
  else {
    watchdog('d4os_io_services_search_parser', t('No host found.'), array(), WATCHDOG_WARNING);
  }
}

function d4os_io_db_os_search_services_parser_checkhost($host) {
  // call the host
  $response = drupal_http_request('http://'. $host->host. ':'. $host->port, array(), 'GET', NULL, 1);
  if ($response->code != 404) {
    watchdog('d4os_io_services_search_parser', t('Host !host was unreachable.'), array('!host' => $host->host. ':'. $host->port), WATCHDOG_ERROR);
    $host_unreachable_limit = variable_get('d4os_io_services_search_parser_host_unreachable_limit', 0);
    if ($host_unreachable_limit > 0 && ($host->failcounter >= $host_unreachable_limit)) {
      d4os_io_db_os_search_hosts_delete($host->host, $host->port);
    }
    else {
      //Setting a "fake" update time so this host will have timeto get back online, so we don't get stuck
      $next = time() + (variable_get('d4os_io_services_search_parser_timer', 15) * 60);
      d4os_io_db_set_active('os_search');
      db_query("UPDATE {hostsregister} SET lastcheck = %d, failcounter = failcounter + 1 WHERE host = '%s' AND port = '%s'", array($next, $host->host, $host->port));
      d4os_io_db_set_active('default');
    }
  }
  else {
    d4os_io_db_set_active('os_search');
    db_query("UPDATE {hostsregister} set failcounter = 0 WHERE host = '%s' AND port = '%s'", array($host->host, $host->port));
    d4os_io_db_set_active('default');
    d4os_io_db_os_search_services_parser_parse($host->host, $host->port);
  }
}

function d4os_io_db_os_search_services_parser_parse($hostname, $port) {
  global $now;
  if ($hostname != "" && $port != "") {
    $next = time() + (variable_get('d4os_io_services_search_parser_timer', 15) * 60);
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

      if (variable_get('d4os_io_services_search_parser_bypass_expire', 0) == 0) {
        d4os_io_db_set_active('os_search');
        $updater = db_query("UPDATE {hostsregister} SET lastcheck = %d WHERE host = '%s' AND port = '%s'", array($next, $hostname, $port));
        d4os_io_db_set_active('default');
      }

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
        $check = db_result(db_query("SELECT count(*) FROM {regions} WHERE regionuuid = '%s'", $regionuuid));
        d4os_io_db_set_active('default');
watchdog('d4os_io_svc_search_parser', t('Region exists ? !check.'), array('!check' => $check), WATCHDOG_ERROR);
        if ($check > 0) {
          d4os_io_db_set_active('os_search');
          db_query("DELETE FROM {regions} WHERE regionuuid = '%s'", $regionuuid);
          db_query("DELETE FROM {parcels} WHERE regionuuid = '%s'", $regionuuid);
          db_query("DELETE FROM {objects} WHERE regionuuid = '%s'", $regionuuid);
          db_query("DELETE FROM {allparcels} WHERE regionUUID = '%s'", $regionuuid);
          db_query("DELETE FROM {parcelsales} WHERE regionUUID = '%s'", $regionuuid);
          d4os_io_db_set_active('default');
        }
        

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

function d4os_io_db_os_search_is_alive() {
  return array(
    'success' => d4os_io_db_mysql_is_alive('os_search', 'os search'),
  );
}

/*
 * search helpers
 */
function d4os_io_os_search_helpers_clean_regions() {
  // get all regions in search base
  $search_regions = array();
  d4os_io_db_set_active('os_search');
  $result = db_query('SELECT regionuuid FROM {regions}');
  while ($row = db_fetch_object($result)) {
    $search_regions[] = $row->regionuuid;
  }
  d4os_io_db_set_active('default');

  // get all regions in grid base
  $grid_regions = array();
  d4os_io_db_set_active('os_grid');
  $result = db_query('SELECT uuid FROM {regions}');
  while ($row = db_fetch_object($result)) {
    $grid_regions[] = $row->uuid;
  }
  d4os_io_db_set_active('default');

  // compare
  $orphan_regions = array_diff($search_regions, $grid_regions);

  if (count($orphan_regions) == 0) {
    return;
  }

  // get parcels uuids by regionuuid
  $parcels = array();
  $placeholders = implode(', ', array_fill(0, count($orphan_regions), "%s"));
  d4os_io_db_set_active('os_search');
  $result = db_query("SELECT parcelUUID FROM {parcels} WHERE regionUUID IN (". $placeholders. ")");
  while ($row = db_fetch_object($result)) {
    $parcels[] = $row->parcelUUID;
  }
  d4os_io_db_set_active('default');
/*
  // delete parcels by regionuuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {parcels} WHERE regionUUID IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete allparcels by regionuuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {allparcels} WHERE regionUUID IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete parcelsales by regionuuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {parcelsales} WHERE regionUUID IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete objects
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {objects} WHERE regionuuid IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete regions
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {regions} WHERE regionuuid IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete classifieds by parcel uuid
  $placeholders = implode(', ', array_fill(0, count($parcels), "%s"));
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {classifieds} WHERE parceluuid IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete objects by parcel uuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {objects} WHERE parceluuid IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete parcels by parcel uuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {parcels} WHERE parcelUUID IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete allparcels by parcel uuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {allparcels} WHERE parcelUUID IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete popularplaces by parcel uuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {popularplaces} WHERE parcelUUID IN (". $placeholders. ")");
  d4os_io_db_set_active('default');

  // delete userpicks by parcel uuid
  d4os_io_db_set_active('os_search');
  $result = db_query("DELETE FROM {popularplaces} WHERE parcelUUID IN (". $placeholders. ")");
  d4os_io_db_set_active('default');
*/
  watchdog('d4os_search', '!nbr_regions regions and !nbr_parcels parcels were deleted in search.', array('!nbr_regions' => $orphan_regions, '!nbr_parcels' => $parcels), $severity = WATCHDOG_NOTICE, $link = NULL)
}
