<?php
/**
 * @package    d4os_ui_events
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Get the UTC time stamp from a date
 */
function d4os_ui_events_get_utc_timestamp_from_date($date) {
  // First get the user's (or system default) timezone info
  $user_timezone_name = date_default_timezone_name(TRUE);

  // Convert the user entered date into a PHP 5 DateTime object
  $local_date = new DateTime($date);

  // Reformat the user entered date into an ISO date that date_make_date() will accept
  $iso_date_string = date_format_date($local_date, 'custom', 'Y-m-d\TH:i:s');

  // Create a DateTime object with the user's timezone info
  $utc_date = date_make_date($iso_date_string, $user_timezone_name, DATE_DATETIME);

  // Change the timezone to UTC
  date_timezone_set($utc_date, timezone_open('UTC'));

  // Format the UTC version of the DateTime for use in node->save()
  $utc_date_string = date_format_date($utc_date, 'custom', 'Y-m-d\TH:i:s');

  // convert to timestamp
  return strtotime($utc_date_string);
}

function d4os_ui_events_get_date_from_utc_timestamp($timestamp, $format='Y-m-d H:i:s') {
  // First get the user's (or system default) timezone info
  $user_timezone_name = date_default_timezone_name(TRUE);

  // convert back to string
  $new_date = date('Y-m-d\TH:i:s', $timestamp);

  // ok, so now get back to the user timezone
  $new_date = date_make_date($new_date, 'UTC', DATE_DATETIME);
  date_timezone_set($new_date, timezone_open($user_timezone_name));

  return date_format_date($new_date, 'custom', $format);
}

function d4os_ui_events_get_duration($time) {
  $duration = array(
    'full'  => $time,
    'days'  => 0,
    'hours' => 0,
    'mins'  => 0,
    'secs'  => 0
  );
  // 78300
  $days = floor($time / 86400);
  if ($days >= 1) {
    $duration['days'] = $days;
    $time = $time - ($days * 86400);
  }
  $hours = floor($time / 3600);
  if ($hours >= 1) {
    $duration['hours'] = $hours;
    $time = $time - ($hours * 3600);
  }
  $mins = floor($time / 60);
  if ($mins >= 1) {
    $duration['mins'] = $mins;
    $time = $time - ($mins * 60);
  }
  $duration['secs'] = $time;
  return $duration;
}

function d4os_ui_events_get_duration_string($time) {
  $duration = d4os_ui_events_get_duration($time);
  // build the duration string
  $duration_string = '';
  if ($duration['days']) {
    $duration_string .= $duration['days']. ' '. t('Days'). ' ';
  }
  if ($duration['hours']) {
    $duration_string .= $duration['hours']. ' '. t('Hours'). ' ';
  }
  if ($duration['mins']) {
    $duration_string .= $duration['mins']. ' '. t('Mins'). ' ';
  }
  if ($duration['secs']) {
    $duration_string .= $duration['secs']. ' '. t('Secs'). ' ';
  }
  return $duration_string;
}

function d4os_ui_events_build_parcels_list($uid) {
  // get the user
  $account = user_load($uid);
  // get the parcels
  $parcels = module_invoke_all('os_events_get_parcels_by_owner', $account->d4os['grid']['account']['uuid']);
  $options = array();
  //$options['none'] = t('Nowhere'));
  if (count($parcels)) {
    foreach ($parcels as $parcel) {
      $field = $parcel['landingpoint']. '|'. $parcel['regionUUID'];
      $options[$field] = $parcel['parcelname'];
    }
  }
  return $options;
}

function d4os_ui_events_get_selected_parcel($region_name, $global_pos) {
  // get region info
  $region = module_invoke_all('os_region_load', array('regionName' => $region_name));
  $region = $region[0];
  // get position
  $position = substr($global_pos, 1, -1); // remove the < >
  $position = explode(',', $position);
  $pos_x = $position[0] - ($region->locX / 256);
  $pos_y = $position[1] - ($region->locY / 256);
  $pos_z = $position[2];
  return $pos_x. '/'. $pos_y. '/'. $pos_z. '|'. $region->uuid;
}

function d4os_ui_events_get_grid_fields() {
  return array(
    'owneruuid',    //s
    'name',         //s
    'eventid',      //d
    'creatoruuid',  //s
    'category',     //d
    'description',  //s
    'dateUTC',      //d
    'duration',     //d
    'covercharge',  //d
    'coveramount',  //d
    'simname',      //s
    'globalPos',    //s
    'eventflags',   //d
    'mature'        //s
  );
}

function d4os_ui_events_get_categories() {
  return array(
    '27' => t('Arts and Culture'),
    '28' => t('Charity/Support Groups'),
    '22' => t('Commercial'),
    '18' => t('Discussion'),
    '26' => t('Education'),
    '24' => t('Games/Contests'),
    '20' => t('Live Music'),
    '29' => t('Miscellaneous'),
    '23' => t('Nightlife/Entertainment'),
    '25' => t('Pageants'),
    '19' => t('Sports'),
  );
}
