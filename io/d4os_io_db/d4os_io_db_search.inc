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
  $inworld_fields = d4os_ui_events_get_inworld_fields();
//drupal_set_message('<pre>'.print_r($event, true).'</pre>', 'warning');return;
  if (is_object($event) && $event->eventid) {
    $grid_query = '';
    $node_query = '';
    $grid_v = array();
    $node_v = array();
    foreach ($event as $key => $value) {
      if (in_array($key, $inworld_fields)) {
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
    db_set_active('os_search');
    $success = db_query("UPDATE {events} SET $grid_query WHERE `eventid` = '%d'", array_merge($grid_v, array($event->eventid)));
    db_set_active('default');
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
      if (in_array($key, $inworld_fields)) {
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

    db_set_active('os_search');
    $success = db_query('INSERT INTO {events} ('. implode(', ', $grid_fields) .') VALUES ('. implode(', ', $grid_s) .')', $grid_values);
    $eventid = db_last_insert_id('events', 'eventid');
    db_set_active('default');
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
  $fields = d4os_ui_events_get_inworld_fields();

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
  //db_set_active('os_events');
  //$result = db_query('SELECT * FROM {events} WHERE '. implode(' AND ', $query), $params);
  $result = db_query('SELECT * FROM {d4os_ui_events} WHERE '. implode(' AND ', $query), $params);
  if ($event = db_fetch_object($result)) {
    //db_set_active('default');
    return $event;
  }
  else {
    //db_set_active('default');
    return FALSE;
  }
}

function d4os_io_db_os_event_load_all() {
  $users = array();
  //db_set_active('os_search');
  //$result = db_query('SELECT * FROM {events}');
  $result = db_query('SELECT * FROM {d4os_ui_events}');
  while ($event = db_fetch_object($result)) {
    $events[] = $event;
  }
  //db_set_active('default');
  return $event;
}

function d4os_io_db_os_event_delete($node) {
  // load the local event
  $eventid = db_result(db_query('SELECT eventid FROM {d4os_ui_events} WHERE nid=%d', $node->nid));
  db_set_active('os_search');
  db_query("DELETE FROM {events} WHERE eventid='%d'", $eventid);
  db_set_active('default');
  db_query("DELETE FROM {d4os_ui_events} WHERE eventid='%d'", $eventid);
}

function d4os_io_db_os_event_delete_revision($vid) {
  db_query('DELETE FROM {d4os_ui_events} WHERE vid = %d', $node->vid);
}

function d4os_io_db_os_events_get_parcels_by_owner($ownerid) {
  db_set_active('os_search');
  $result = db_query("SELECT regionUUID, parcelname, landingpoint FROM {allparcels} WHERE ownerUUID = '%s'", $ownerid);
  $parcels = array();
  while($parcel = db_fetch_array($result)) {
    $parcels[] = $parcel;
  }
  db_set_active('default');
  if (count($parcels)) {
    return $parcels;
  }
  return FALSE;
}
