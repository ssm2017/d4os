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

function d4os_io_db_os_event_save($event) {
// TODO : check if success before saving the node itself
  // convert array to object
  $event = is_array($event) ? (object)$event : $event;
  // get inworld fields
  $grid_fields = d4os_ui_events_get_grid_fields();
//drupal_set_message('<pre>'.print_r($event, true).'</pre>', 'warning');return;
  if (is_object($event) && $event->eventid) {
    $grid_query = '';
    $node_query = '';
    $grid_v = array();
    $node_v = array();
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
            $node_query .= "$key = '%s', ";
            $node_v[]    = $value;
            break;
          case 'category':
          case 'dateUTC':
          case 'duration':
          case 'covercharge':
          case 'coveramount':
          case 'eventflags':
            $grid_query .= "$key = %d, ";
            $grid_v[]    = $value;
            $node_query .= "$key = %d, ";
            $node_v[]    = $value;
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
    $node_query = substr($node_query, 0, -2);
    // make the query
    d4os_io_db_set_active('os_search');
    $success = db_query("UPDATE {events} SET $grid_query WHERE `eventid` = '%d'", array_merge($grid_v, array($event->eventid)));
    d4os_io_db_set_active('default');
    if (!$success) {
      // The query failed - better to abort the save than risk further data loss.
      drupal_set_message(t('Error saving the event to the grid database'), 'error');
      return FALSE;
    }
    $success = db_query("UPDATE {d4os_ui_events} SET $node_query WHERE `eventid` = '%d'", array_merge($node_v, array($event->eventid)));
    if (!$success) {
      drupal_set_message(t('Error saving the event to the website database'), 'error');
      // The query failed - better to abort the save than risk further data loss.
      return FALSE;
    }
  }
  else {
    $grid_fields = array();
    $grid_values = array();
    $grid_s = array();
    $node_fields = array();
    $node_values = array();
    $node_s = array();
    // create the new user
    foreach ($event as $key => $value) {
      if (in_array($key, $grid_fields)) {
        switch ($key) {
          case 'owneruuid':
          case 'creatoruuid':
          case 'simname':
          case 'globalPos':
          case 'mature':
            $grid_fields[] = $key;
            $grid_values[] = $value;
            $grid_s[] = "'%s'";
            $node_fields[] = $key;
            $node_values[] = $value;
            $node_s[] = "'%s'";
            break;
          case 'category':
          case 'dateUTC':
          case 'duration':
          case 'covercharge':
          case 'coveramount':
          case 'eventflags':
            $grid_fields[] = $key;
            $grid_values[] = $value;
            $grid_s[] = "%d";
            $node_fields[] = $key;
            $node_values[] = $value;
            $node_s[] = "%d";
            break;
          case 'name':
          case 'description':
            $grid_fields[] = $key;
            $grid_values[] = $value;
            $grid_s[] = "'%s'";
            break;
        }
      }
    }

    d4os_io_db_set_active('os_search');
    $success = db_query('INSERT INTO {events} ('. implode(', ', $grid_fields) .') VALUES ('. implode(', ', $grid_s) .')', $grid_values);
    $eventid = db_last_insert_id('events', 'eventid');
    d4os_io_db_set_active('default');
    if (!$success) {
      // The query failed - better to abort the save than risk further data loss.
      drupal_set_message(t('Error saving the event to the grid database'), 'error');
      return FALSE;
    }
    // save the node item
    $node_fields[] = 'eventid';
    $node_values[] = $eventid;
    $node_s[] = '%d';
    $node_fields[] = 'vid';
    $node_values[] = $event->vid;
    $node_s[] = '%d';
    $node_fields[] = 'nid';
    $node_values[] = $event->nid;
    $node_s[] = '%d';
    $success = db_query('INSERT INTO {d4os_ui_events} ('. implode(', ', $node_fields) .') VALUES ('. implode(', ', $node_s) .')', $node_values);
    if (!$success) {
      drupal_set_message(t('Error saving the event to the website database'), 'error');
      // The query failed - better to abort the save than risk further data loss.
      return FALSE;
    }
  }

  // Refresh event object.
  $event = d4os_io_db_os_event_load($event);

  return $event;
}


function d4os_io_db_os_event_load($array = array()) {
  // get inworld fields
  $fields = d4os_ui_events_get_grid_fields();

  // Dynamically compose a SQL query:
  $query = array();
  $params = array();

  if (!is_array($array)) {
    return FALSE;
  }

  foreach ($array as $key => $value) {
    if (in_array($key, $fields)) {
      switch ($key) {
        case 'owneruuid':
        //case 'name':
        case 'creatoruuid':
        //case 'description':
        case 'simname':
        case 'globalPos':
        case 'mature':
          $query[]= "$key = '%s'";
          $params[] = $value;
          break;
        case 'category':
        case 'dateUTC':
        case 'duration':
        case 'covercharge':
        case 'coveramount':
        case 'eventflags':
          $query[]= "$key = %d";
          $params[] = $value;
          break;
      }
    }
    else if ($key == 'nid') {
      $query[]= "$key = %d";
      $params[] = $value;
    }
  }
  //d4os_io_db_set_active('os_events');
  //$result = db_query('SELECT * FROM {events} WHERE '. implode(' AND ', $query), $params);
  $result = db_query('SELECT * FROM {d4os_ui_events} WHERE '. implode(' AND ', $query), $params);
  if ($event = db_fetch_object($result)) {
    //d4os_io_db_set_active('default');
    return $event;
  }
  else {
    //d4os_io_db_set_active('default');
    return FALSE;
  }
}

function d4os_io_db_os_event_load_all() {
  $users = array();
  //d4os_io_db_set_active('os_search');
  //$result = db_query('SELECT * FROM {events}');
  $result = db_query('SELECT * FROM {d4os_ui_events}');
  while ($event = db_fetch_object($result)) {
    $events[] = $event;
  }
  //d4os_io_db_set_active('default');
  return $event;
}

function d4os_io_db_os_event_delete($node) {
  // load the local event
  $eventid = db_result(db_query('SELECT eventid FROM {d4os_ui_events} WHERE nid=%d', $node->nid));
  d4os_io_db_set_active('os_search');
  db_query("DELETE FROM {events} WHERE eventid='%d'", $eventid);
  d4os_io_db_set_active('default');
  db_query("DELETE FROM {d4os_ui_events} WHERE eventid='%d'", $eventid);
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
 * search services
 */

function d4os_io_db_os_search_services_dir_places_query($params) {
  $category = $params['category'];
  $text     = $params['search'];
  $start    = $params['start'];
  
  d4os_io_db_set_active('os_search');
  if ($category != -1) {
    $data = array($category, $text, $text);
    $result = db_query_range("SELECT * FROM {parcels} WHERE ".
                             "(searchcategory = -1 or searchcategory = '%s') ".
                             "AND (parcelname like '%%%s%%' ".
                             "OR description like '%%%s%%') ".
                             "ORDER BY dwell desc, parcelname",
                             $data, 0+$query_start, 100);
  }
  else {
    $data = array($text, $text);
    $result = db_query_range("SELECT * FROM {parcels} WHERE ".
                             "parcelname like '%%%s%%' ".
                             "OR description like '%%%s%%' ".
                             "ORDER BY dwell desc, parcelname",
                             $data, 0+$query_start, 100);
  }
  $data = array();
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
      "description"     => $row["description"],
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