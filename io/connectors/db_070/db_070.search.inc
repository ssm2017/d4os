<?php

/**
 * @package    d4os_io db 070
 * @copyright Copyright (C) 2010-2013 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
class D4OS_IO_db_070_Search implements D4OS_IO_Search_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  /**
   * Check if the database is answering
   * @return nothing
   */
  function ping() {
    $this->response->success = d4os_io_db_070_mysql_is_alive('os_search', 'Search');
    return;
  }

  function get_user_parcels_count($uuid) {
    d4os_io_db_070_set_active('os_search');
    $qty = db_query("SELECT count(parcelUUID) FROM {allparcels} WHERE ownerUUID=:uuid", array(':uuid' => $uuid))->fetchField();
    d4os_io_db_070_set_active('default');
    return $qty;
  }

  /**
   * Get a list of the parcel owned by an avatar
   * @param string $uuid of the parcels's owner
   * @return array An array of the parcels
   */
  function get_user_parcels($uuid) {
    $parcels = array();
    d4os_io_db_070_set_active('os_search');
    $results = db_query("SELECT regionUUID, parcelname, landingpoint FROM {allparcels} WHERE ownerUUID = :uuid", array(':uuid' => $uuid));
    foreach ($results as $result) {
      $parcels[] = $result;
    }
    d4os_io_db_070_set_active('default');
    return $parcels;
  }

  /**
   * Get an event's data
   * @param int $event_id
   * @return object An object of the event
   */
  function get_event($event_id) {
    d4os_io_db_070_set_active('os_search');
    $event = db_query("SELECT * FROM {events} WHERE eventid = :event_id", array(':event_id' => $event_id))->fetch();
    d4os_io_db_070_set_active('default');
    return $event;
  }

  /**
   * Save an event in the grid database
   * @param type $values
   * @return int The event id or NULL if error
   */
  function save_event($values) {
    if (!is_array($values)) {
      return;
    }

    $data = array();
    foreach ($values as $key => $value) {
      switch ($key) {
        case 'owneruuid': // char(36)
        case 'name': // varchar(255)
        case 'eventid': // binary(1)
        case 'creatoruuid': // varchar(255)
        case 'category': // int(10)
        case 'description': // text
        case 'dateUTC': // int(10)
        case 'duration': // int(10)
        case 'covercharge': // tinyint(3)
        case 'coveramount': // int(10)
        case 'simname': // varchar(255)
        case 'globalPos': // varchar(255)
        case 'eventflags': // tinyint(3)
          $data[$key] = $value;
          break;
      }
    }

    // update or insert ?
    if (isset($data['eventid']) && !is_null($data['eventid'])) {
      d4os_io_db_070_set_active('os_search');
      $saved = db_update('events')
          ->fields($data)
          ->condition('eventid', $data['eventid'])
          ->execute();
      $event_id = $data['eventid'];
      d4os_io_db_070_set_active('default');
      drupal_set_message(t('Event updated on the grid'));
    }
    else {
      d4os_io_db_070_set_active('os_search');
      $saved = db_insert('events')
          ->fields($data)
          ->execute();
      $event_id = (int)$saved;
      d4os_io_db_070_set_active('default');
      drupal_set_message(t('Event added to the grid.'));
    }

    // link the node
    if ($event_id !== 0) {
      d4os_ui_events_set_uuid($values['nid'], $event_id);
    }

    return $event_id;
  }

  /**
   * Delete an event in the grid database
   * @param int $event_id
   */
  function delete_event($event_id) {
    d4os_io_db_070_set_active('os_search');
    $result = db_delete('events')->condition('eventid', $event_id)->execute();
    $done = $result;
    d4os_io_db_070_set_active('default');
    if ($done) {
      drupal_set_message(t('Event deleted on the grid.'));
      // delete the event link on the drupal side
      db_delete('d4os_ui_events')->condition('event_id', $event_id)->execute();
    }
    else {
      drupal_set_message(t('There was an error when deleting event on the grid.'));
    }
  }

  /**
   * Delete a user in the grid database
   * @param char $uuid
   */
  function delete_user($uuid) {
    // delete the user events
    d4os_io_db_070_set_active('os_search');
    db_delete('events')->condition('owneruuid', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('User events deleted.'));
  }

}
