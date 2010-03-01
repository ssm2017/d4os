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

function d4os_io_services_search_dir_places_query($text, $category, $query_start=0) {
  $pieces = split(" ", $text);
  $text = join("%", $pieces);
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms'),
    );
  }

  db_set_active('os_search');
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
  db_set_active('default');
  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_services_search_dir_places_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_popular_query($flags) {
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
  
  db_set_active('os_search');
  $result = mysql_query("SELECT * FROM {popularplaces}" . $where);
  
  $data = array();
  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "parcel_id" => $row["infoUUID"],
      "name"      => $row["name"],
      "dwell"     => $row["dwell"]
    );
  }
  db_set_active('default');
  
  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_services_search_dir_popular_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_land_query($flags, $type, $price, $area, $query_start) {

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

  db_set_active('os_search');
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
  db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_services_search_dir_land_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_events_query($text="", $flags="", $query_start=0) {
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms.'),
    );
  }

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

  db_set_active('os_search');
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
  db_set_active('default');
  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_services_search_dir_events_query_access() {
  return TRUE;
}

function d4os_io_services_search_dir_classified_query ($text, $flags, $category, $query_start) {
  
  if ($text == "%%%") {
    return array(
      'success'      => FALSE,
      'errorMessage' => t('Invalid search terms.'),
    );
  }

  if ($category <> 0) $terms[] = "category = ".$category."";

  $where = "";

  if (count($terms) > 0) {
    $where = " where " . join(" and ", $terms);
  }

  db_set_active('os_search');
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
  db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_services_search_dir_classified_query_access() {
  return TRUE;
}

function d4os_io_services_search_event_info_query($eventID=0) {

  db_set_active('os_search');
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
  db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_services_search_event_info_query_access() {
  return TRUE;
}

function d4os_io_services_search_classifieds_info_query($classifiedID) {

  db_set_active('os_search');
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
  db_set_active('default');

  return array(
    'success'      => True,
    'errorMessage' => "",
    'data'         => $data
  );
}

function d4os_io_services_search_classifieds_info_query_access() {
  return TRUE;
}