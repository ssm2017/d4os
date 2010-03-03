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

function d4os_io_db_os_group_save($params) {
  // include the groups file
  module_load_include('services.inc', 'd4os_io_services_groups');
  $success = FALSE;
  // check if the group already exists
  $group = d4os_io_db_os_group_load($params);

  if (is_array($group)) {
    $params['GroupID']     = $group['GroupID'];
    $params['FounderID']   = $group['FounderID'];
    $params['OwnerRoleID'] = $group['OwnerRoleID'];
    $params['InsigniaID']  = $group['InsigniaID'];
    $group = d4os_io_services_groups_updateGroup($params, FALSE);
    if (is_array($group)) {
      $success = TRUE;
      $group['action'] = 'update';
    }
  }
  else {
    // get the user
    $account = user_load($params['uid']);
    if (!isset($account->grid_UUID) || empty($account->grid_UUID)) {
      drupal_set_message(t('This user is not registered in the grid. Inworld group was not created for this user.'), 'error');
      return FALSE;
    }
    $params['GroupID']        = d4os_build_uuid();
    $params['InsigniaID']     = UUID_ZERO;
    $params['FounderID']      = $account->grid_UUID;
    $params['OwnerRoleID']    = d4os_build_uuid();
    $params['EveryonePowers'] = 62672565501952;
    $params['OwnersPowers']   = 349644697632766;
    $group = d4os_io_services_groups_createGroup($params, FALSE);
    $success = db_query("INSERT INTO {d4os_ui_groups} (nid, GroupID) VALUES (%d, '%s')", array($params['nid'], $params['GroupID']));
    $group['action'] = 'add';
  }
  if ($success) {
    $group['nid'] = $params['nid'];
    return $group;
  }
  else {
    return FALSE;
  }
}

function d4os_io_db_os_group_load($params) {
  $fields = array();
  $values = array();
  if (isset($params['nid'])) {
    $fields[] = "nid = %d";
    $values[] = $params['nid'];
  }
  if (isset($group['GroupID'])) {
    $fields[] = "GroupID = '%s'";
    $values[] = $group['GroupID'];
  }
  $where = '';
  if (count($fields)) {
    $where = ' WHERE '. implode(' AND ', $fields);
  }
  $group = db_fetch_array(db_query("SELECT * FROM {d4os_ui_groups}". $where, $values));
  if (is_array($group)) {
    // include the groups file
    module_load_include('services.inc', 'd4os_io_services_groups');
    // get the inworld group
    $grid_group = d4os_io_services_groups_getGroup($group, FALSE);
    if (!isset($grid_group['succeed']) || $grid_group['succeed'] != 'false') {
      $grid_group['nid'] = $group['nid'];
      return $grid_group;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

function d4os_io_db_os_group_load_all() {
  $groups = array();
  db_set_active('os_groups');
  $result = db_query('SELECT *, Name AS title FROM {osgroup}');
  while ($group = db_fetch_array($result)) {
    $groups[] = $group;
  }
  db_set_active('default');
  return $groups;
}

function d4os_io_db_os_group_delete($params) {
  // get the group
  $group = d4os_io_db_os_group_load($params);
  db_query("DELETE FROM {d4os_ui_groups} WHERE nid = %d". $group['nid']);
  db_set_active('os_groups');
  db_query("DELETE FROM {osagent} WHERE ActiveGroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroupinvite} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroupmembership} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroupnotice} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgrouprolemembership} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osrole} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroup} WHERE GroupID = '%s'", array($group['GroupID']));
  db_set_active('default');
}

function d4os_io_db_os_group_subscribe($params) {
  // include the groups file
  module_load_include('services.inc', 'd4os_io_services_groups');
  return d4os_io_services_groups_addAgentToGroup($params, FALSE);
}

function d4os_io_db_os_group_unsubscribe($params) {
  // include the groups file
  module_load_include('services.inc', 'd4os_io_services_groups');
  return d4os_io_services_groups_removeAgentFromGroup($params, FALSE);
}

function d4os_io_db_os_group_grid_create($params) {
  // get the content type to build nodes
  $node_type = variable_get('d4os_ui_groups_type', NULL);
  // get the user
  $account = module_invoke_all('os_user_load', array('UUID' => $params['FounderID']));
  $account = $account[0];
  $account = user_load(array('name' => $account->username. ' '. $account->lastname));
  // build the node
  $node = new stdClass;
  $node->uid            = $account->uid;
  $node->type           = $node_type;
  $node->title          = $params['Name'];
  $node->body           = $params['Charter'];
  $node->status         = 1;
  $node->og_description = $params['Name'];
  $node->og_selective   = $params['OpenEnrollment'];
  $node->og_register    = 0;
  $node->og_directory   = $params['AllowPublish'];
  $node->og_private     = $params['OpenEnrollment'];
  $node->og_theme       = NULL;
  $node->inworld        = TRUE;
  node_save($node);
  db_query("INSERT INTO {d4os_ui_groups} (nid, GroupID) VALUES (%d, '%s')", array($node->nid, $params['GroupID']));
  global $logLevel;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'os_group_grid_create',
    );
    $log[] = array(
      'name' => 'input values',
      'type' => 'array',
      'data' => $params,
    );
    $log[] = array(
      'name' => 'user',
      'type' => 'array',
      'data' => $account,
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $node,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
}

function d4os_io_db_os_group_grid_update($params) {
  // get the node
  $nid = db_result(db_query("SELECT nid FROM {d4os_ui_groups} WHERE GroupID = '%s'", array($params['GroupID'])));
  $node = node_load($nid);
  // build the node
  $node->title          = $params['Name'];
  $node->body           = $params['Charter'];
  $node->og_selective   = $params['OpenEnrollment'];
  $node->og_directory   = $params['AllowPublish'];
  $node->og_private     = $params['OpenEnrollment'];
  $node->inworld        = TRUE;
  node_save($node);
  global $logLevel;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'os_group_grid_update',
    );
    $log[] = array(
      'name' => 'input values',
      'type' => 'array',
      'data' => $params,
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $node,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
}