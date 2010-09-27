<?php
/**
 * @package    d4os_io_db
 * @subpackage messages
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Save an offline message
 * @param Array of search params
 * @return Boolean TRUE if success or FALSE if error
 */
function d4os_io_db_os_services_offline_message_savemessage($params) {
  $uuid     = $params['toAgentID'];
  $message  = $params['message'];

  d4os_io_db_set_active('os_offline_message');
  $success = db_query("INSERT INTO {offline_msgs} (uuid, message) VALUES ('%s', '%s')", array($uuid, $message));
  d4os_io_db_set_active('default');

  if ($success) {
    if (variable_get('d4os_io_services_offline_message_send_mails', 0)) {
      d4os_io_services_offline_message_mail_send($params);
    }
    return array(
      'success' => TRUE,
    );
  }
  return array(
    'success' => FALSE,
  );
}

/**
 * Return a list of saved messages
 * @param Array of search params
 * @return Array like array('success'=>true, 'data'=>$data);
 */
function d4os_io_db_os_services_offline_message_retrievemessages($params) {
  $uuid = $params['Guid'];

  // get the messages
  $messages_list = array();
  d4os_io_db_set_active('os_offline_message');
  $messages = db_query("SELECT * FROM {offline_msgs} WHERE uuid = '%s'", array($uuid));
  while ($message = db_fetch_object($messages)) {
    $messages_list[] = $message->message;
  }
  d4os_io_db_set_active('default');

  // delete the messages
  d4os_io_db_set_active('os_offline_message');
  db_query("DELETE FROM {offline_msgs} WHERE uuid = '%s'", array($uuid));
  d4os_io_db_set_active('default');

  return array(
    'success' => TRUE,
    'data'    => $messages_list,
  );
}

/**
 * Delete messages for a user by uuid
 * @param uuid of the user to delete messages
 */
function d4os_io_db_offline_message_user_delete($uuid) {
  d4os_io_db_set_active('os_offline_message');
  db_query("DELETE FROM {offline_msgs} WHERE uuid = '%s'", array($uuid));
  d4os_io_db_set_active('default');
}


/**
 * Check if the offline_messages database is responding
 * @return Array like array('success'=>true or false);
 */
function d4os_io_db_os_offline_messages_is_alive() {
  return array(
    'success' => d4os_io_db_mysql_is_alive('os_offline_messages', 'os offline messages'),
  );
}
