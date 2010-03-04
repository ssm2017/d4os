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

/*
 * wmlrpc groups methods
 */
function d4os_io_db_os_groups_services_createGroup($params) {
  $groupID        = $params["GroupID"];
  $name           = addslashes( $params["Name"] );
  $charter        = addslashes( $params["Charter"] );
  $insigniaID     = $params["InsigniaID"];
  $founderID      = $params["FounderID"];
  $membershipFee  = $params["MembershipFee"];
  $openEnrollment = $params["OpenEnrollment"];
  $showInList     = $params["ShowInList"];
  $allowPublish   = $params["AllowPublish"];
  $maturePublish  = $params["MaturePublish"];
  $ownerRoleID    = $params["OwnerRoleID"];
  $everyonePowers = $params["EveryonePowers"];
  $ownersPowers   = $params["OwnersPowers"];

  // Create group
  $sql = "INSERT INTO {osgroup}
  (GroupID, Name, Charter, InsigniaID, FounderID, MembershipFee, OpenEnrollment, ShowInList, AllowPublish, MaturePublish, OwnerRoleID)
  VALUES
  ('%s', '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, '%s')";
  $values = array($groupID, $name, $charter, $insigniaID, $founderID, $membershipFee, $openEnrollment, $showInList, $allowPublish, $maturePublish, $ownerRoleID);

  db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');
  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_addRoleToGroup($params) {
  $everyonePowers = 8796495740928; // This should now be fixed, when libomv was updated...		
  $groupID = $params['GroupID'];
  $roleID  = $params['RoleID'];
  $name    = addslashes( $params['Name'] );
  $desc    = addslashes( $params['Description'] );
  $title   = addslashes( $params['Title'] );
  $powers  = $params['Powers'];

  if (!isset($powers) || ($powers == 0) || ($powers == '')) {
    $powers = $everyonePowers;
  }

  $sql = " INSERT INTO {osrole} (GroupID, RoleID, Name, Description, Title, Powers) VALUES "
    ." ('%s', '%s', '%s', '%s', '%s', %d)";
  $data = array($groupID, $roleID, $name, $desc, $title, $powers);

  db_set_active('os_groups');
  if (!db_query($sql, $data)) {
    db_set_active('default');
    return array(
      'error'   => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'method'  => 'd4os_io_services_groups_addRoleToGroup',
      'params'  => var_export($params, TRUE)
    );
  }
  db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_updateGroupRole($params) {
  $groupID = $params['GroupID'];
  $roleID  = $params['RoleID'];
  $name    = addslashes( $params['Name'] );
  $desc    = addslashes( $params['Description'] );
  $title   = addslashes( $params['Title'] );
  $powers  = $params['Powers'];
  $fields = array();
  $values = array();

  $sql = " UPDATE {osrole} SET ";
  $fields[] = "RoleID = '%s'";
  $values[] = $roleID;

  if (isset($params['Name'])) {
    $fields[] = "name = '%s'";
    $values[] = $name;
  }

  if (isset($params['Description'])) {
    $fields[] = "Description = '%s'";
    $values[] = $desc;
  }

  if (isset($params['Title'])) {
    $fields[] = "Title = '%s'";
    $values[] = $title;
  }

  if (isset($params['Powers'])) {
    $fields[] = "Powers = %d";
    $values[] = $powers;
  }

  $sql .= implode(',', $fields);
  $sql .= " WHERE GroupID = '%s' AND RoleID = '%s'";
  $values[] = $groupID;
  $values[] = $roleID;

  db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_removeRoleFromGroup($params) {
  $groupID = $params['GroupID'];
  $roleID  = $params['RoleID'];
  /// 1. Remove all members from Role
  /// 2. Set selected Role to uuidZero for anyone that had the role selected
  /// 3. Delete roll

  $sql = "DELETE FROM {osgrouprolemembership} WHERE GroupID = '%s' AND RoleID = '%s'";
  $values = array($groupID, $roleID);
  db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $sql = "UPDATE {osgroupmembership} SET SelectedRoleID = '%s' WHERE GroupID = '%s' AND SelectedRoleID = '%s'";
  $values = array(UUID_ZERO, $groupID, $roleID);
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');

  $sql = "DELETE FROM {osrole} WHERE GroupID = '$groupID' AND RoleID = '$roleID'";
  $values = array($groupID, $roleID);
  db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');
  return array("success" => "true");
}

function d4os_io_db_os_groups_services_getGroup($params) {
  $sql = " SELECT osgroup.GroupID, osgroup.Name, Charter, InsigniaID, FounderID, MembershipFee, OpenEnrollment, ShowInList, AllowPublish, MaturePublish, OwnerRoleID"
  ." , count(osrole.RoleID) as GroupRolesCount, count(osgroupmembership.AgentID) as GroupMembershipCount "
  ." FROM {osgroup} "
  ." LEFT JOIN {osrole} ON (osgroup.GroupID = osrole.GroupID)"
  ." LEFT JOIN {osgroupmembership} ON (osgroup.GroupID = osgroupmembership.GroupID)"
  ." WHERE ";
  $values = array();
  if(isset($params['GroupID'])) {
    $sql .= "osgroup.GroupID = '%s'";
    $values[] = $params['GroupID'];
  }
  else if (isset($params['Name'])) {
    $sql .= "osgroup.Name = '%s'";
    $values[] = addslashes($params['Name']);
  }
  else {
    return array("error" => "Must specify GroupID or Name");
  }

  $sql .= " GROUP BY osgroup.GroupID, osgroup.name, charter, insigniaID, founderID, membershipFee, openEnrollment, showInList, allowPublish, maturePublish, ownerRoleID";

  db_set_active('os_groups');
  $result = db_query($sql, $values);
  db_set_active('default');

  if (!$result) {
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  db_set_active('os_groups');
  $group = db_fetch_array($result);
  db_set_active('default');
  if (!$group) {
    return array(
      'succeed' => 'false',
      'error' => 'Group Not Found',
      'params' => var_export($params, TRUE),
      'sql' => $sql);
  }
  return $group;
}

function d4os_io_db_os_groups_services_updateGroup($params) {
  $name           = $params['Name'];
  $groupID        = $params["GroupID"];
  $charter        = addslashes( $params["Charter"] );
  $insigniaID     = $params["InsigniaID"];
  $membershipFee  = $params["MembershipFee"];
  $openEnrollment = $params["OpenEnrollment"];
  $showInList     = $params["ShowInList"];
  $allowPublish   = $params["AllowPublish"];
  $maturePublish  = $params["MaturePublish"];

  // Create group
  $sql = "UPDATE {osgroup}
  SET
  Name = '%s',
  Charter = '%s',
  InsigniaID = '%s',
  MembershipFee = %d,
  OpenEnrollment= %d,
  ShowInList    = %d,
  AllowPublish  = %d,
  MaturePublish = %d
  WHERE
  GroupID = '%s'";
  $values = array($name, $charter, $insigniaID, $membershipFee, $openEnrollment, $showInList, $allowPublish, $maturePublish, $groupID);

  db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');

  if ($inworld) {
    module_invoke_all('os_group_grid_update', $params);
  }

  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_findGroups($params) {
  $search = addslashes($params['Search']);

  $sql = " SELECT osgroup.GroupID, osgroup.Name, count(osgroupmembership.AgentID) as Members "
  ." FROM {osgroup} LEFT JOIN {osgroupmembership} ON (osgroup.GroupID = osgroupmembership.GroupID) "
  ." WHERE "
  ." (    MATCH (osgroup.name) AGAINST ('%s' IN BOOLEAN MODE)"
  ."   OR osgroup.name LIKE '%%s%'"
  ."   OR osgroup.name REGEXP '%s'"
  ." ) AND ShowInList = 1" 
  ." GROUP BY osgroup.GroupID, osgroup.Name";
  $values = array($search, $search, $search);

  db_set_active('os_groups');
  $result = db_query($sql, $values);

  if (!$result) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  
  $results = array();
  $num_rows = FALSE;
  while ($row = db_fetch_array($result)) {
    $groupID = $row['GroupID'];
    $results[$groupID] = $row;
    $num_rows = TRUE;
  }
  db_set_active('default');

  if(!$num_rows) {
    return array(
      'succeed' => 'false',
      'error' => 'No groups found.',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  return array('results' => $results, 'success' => 'true');
}

function d4os_io_db_os_groups_services_setAgentActiveGroup($params) {
  $agentID = $params['AgentID'];
  $groupID = $params['GroupID'];

  $sql = " UPDATE {osagent} "
  ." SET ActiveGroupID = '%s'"
  ." WHERE AgentID = '%s'";
  $values = array($groupID, $agentID);
  db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  if( db_affected_rows() == 0 ) {
    $sql = " INSERT INTO {osagent} (ActiveGroupID, AgentID) VALUES ('%s', '%s')";
  
    if (!db_query($sql, $values)) {
      db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
  }
  db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_addAgentToGroup($params) {
  $groupID = $params["GroupID"];
  $agentID = $params["AgentID"];
  // Group is not open enrollment, check if the specified agentid has an invite
  $sql = " SELECT GroupID, RoleID, AgentID FROM {osgroupinvite}"
  ." WHERE osgroupinvite.AgentID = '%s' AND osgroupinvite.GroupID = '%s'";
  $values = array($agentID, $groupID);

  db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $inviteInfo = db_fetch_array($results);
  db_set_active('default');
  if (!is_array($inviteInfo)) {
    return array(
      'error' => t("Could not add agent to group "),
      'params' => var_export($params, TRUE)
    );
  }
  return $inviteInfo;
}

function d4os_io_db_os_groups_services_addAgentToGroup_2($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  
  $roleID  = UUID_ZERO;
  if (isset($params["RoleID"])) {
    $roleID = $params["RoleID"];
  }
  // Check if agent already a member
  $sql = " SELECT count(AgentID) as isMember FROM {osgroupmembership} WHERE AgentID = '%s' AND GroupID = '%s'";
  $values = array($agentID, $groupID);
  db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  $members_count = db_result($result);

  // If not a member, add membership, select role (defaults to uuidZero, or everyone role)
  if ($members_count == 0) {
    $sql = " INSERT INTO {osgroupmembership} (GroupID, AgentID, Contribution, ListInProfile, AcceptNotices, SelectedRoleID) VALUES ('%s','%s', 0, 1, 1,'%s')";
    $values = array($groupID, $agentID, $roleID);

    if (!db_query($sql, $values)) {
      db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
  }
  db_set_active('default');
  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_removeAgentFromGroup($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];

  // 1. If group is agent's active group, change active group to uuidZero
  // 2. Remove Agent from group (osgroupmembership)
  // 3. Remove Agent from all of the groups roles (osgrouprolemembership)

  $sql = " UPDATE {osagent} "
  ." SET ActiveGroupID = 'UUID_ZERO'"
  ." WHERE AgentID = '%s' AND ActiveGroupID = '%s'";
  $values = array($agentID, $groupID);
  db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $sql = " DELETE FROM {osgroupmembership} "
  ." WHERE AgentID = '%s' AND GroupID = '%s'";
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $sql = " DELETE FROM {osgrouprolemembership} "
  ." WHERE AgentID = '%s' AND GroupID = '%s'";
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');
  return array("success" => "true");
}

function d4os_io_db_os_groups_services_addAgentToGroupRole($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];

  // Check if agent already a member
  $sql = " SELECT count(AgentID) as isMember FROM {osgrouprolemembership} WHERE AgentID = '%s' AND RoleID = '%s' AND GroupID = '%s'";
  $values = array($agentID, $roleID, $groupID);
  db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  if (db_result($result, 0) == 0) {
    $sql = " INSERT INTO {osgrouprolemembership} (AgentID, RoleID, GroupID) VALUES ('%s', '%s', '%s')";
    
    if (!db_query($sql, $values)) {
      db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
  }
  db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_addAgentToGroupRole_2($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];
  
  // Check if being assigned to Owners role, assignments to an owners role can only be requested by owners.
  $sql = " SELECT OwnerRoleID, osgrouprolemembership.AgentID "
  ." FROM {osgroup} LEFT JOIN {osgrouprolemembership} ON (osgroup.GroupID = osgrouprolemembership.GroupID AND osgroup.OwnerRoleID = osgrouprolemembership.RoleID) "
  ." WHERE osgrouprolemembership.AgentID = '%s' AND osgroup.GroupID = '%s'";
  $values = array($requestingAgent, $groupID);
  db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  $ownerRoleInfo = db_fetch_array($results);
  db_set_active('default');
  if (!is_array($ownerRoleInfo)) {
    return array(
      'error' => t("Could not add afent to group role"),
      'params' => var_export($params, TRUE)
    );
  }
  return $ownerRoleInfo;
}

function d4os_io_db_os_groups_services_removeAgentFromGroupRole($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID  = $params["RoleID"];
  // If agent has this role selected, change their selection to everyone (uuidZero) role
  $sql = " UPDATE {osgroupmembership} SET SelectedRoleID = '%s' WHERE AgentID = '%s' AND GroupID = '%s' AND SelectedRoleID = '%s'";
  $values = array(UUID_ZERO, $agentID, $groupID, $roleID);
  db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  
  $sql = " DELETE FROM {osgrouprolemembership} WHERE AgentID = '%s' AND GroupID = '%s' AND RoleID = '%s'";
  $values = array($agentID, $groupID, $roleID);
  if (!db_query($sql, $values)) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_setAgentGroupSelectedRole($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];

  $sql = " UPDATE {osgroupmembership} SET SelectedRoleID = '%s' WHERE AgentID = '%s' AND GroupID = '%s'";
  $values = array($roleID, $agentID, $groupID);
  db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');

  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_getAgentGroupMembership($params) {
  $groupID = $params['GroupID'];
  $agentID = $params['AgentID'];

  $sql = " SELECT osgroup.GroupID, osgroup.Name as GroupName, osgroup.Charter, osgroup.InsigniaID, osgroup.FounderID, osgroup.MembershipFee, osgroup.OpenEnrollment, osgroup.ShowInList, osgroup.AllowPublish, osgroup.MaturePublish"
  ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
  ." , osgroupmembership.SelectedRoleID, osrole.Title"
  ." , osagent.ActiveGroupID "
  ." FROM {osgroup} JOIN {osgroupmembership} ON (osgroup.GroupID = osgroupmembership.GroupID)"
  ."              JOIN {osrole} ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
  ."              JOIN {osagent} ON (osagent.AgentID = osgroupmembership.AgentID)"
  ." WHERE osgroup.GroupID = '%sa' AND osgroupmembership.AgentID = '%s'";
  $values = array($groupID, $agentID);
  db_set_active('os_groups');
  $groupmembershipResult = db_query($sql, $values);
  if (!$groupmembershipResult) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $groupMembershipInfo = db_fetch_array($groupmembershipResult);

  if (!is_array($groupMembershipInfo)) {
    db_set_active('default');
    return array(
      'succeed' => 'false',
      'error'   => 'None Found',
      'params'  => var_export($params, TRUE),
      'sql'     => $sql
    );
  }

  $sql = " SELECT BIT_OR(osrole.Powers) AS GroupPowers"
  ." FROM {osgrouprolemembership} JOIN {osrole} ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
  ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";
  
  $groupPowersResult = db_query($sql, $values);
  if (!$groupPowersResult) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  $groupPowersInfo = db_fetch_array($groupPowersResult);
  db_set_active('default');

  return array_merge($groupMembershipInfo, $groupPowersInfo);
}

function d4os_io_db_os_groups_services_getAgentGroupMemberships($params) {
  $agentID = $params['AgentID'];
  
  $sql = " SELECT osgroup.GroupID, osgroup.Name as GroupName, osgroup.Charter, osgroup.InsigniaID, osgroup.FounderID, osgroup.MembershipFee, osgroup.OpenEnrollment, osgroup.ShowInList, osgroup.AllowPublish, osgroup.MaturePublish"
  ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
  ." , osgroupmembership.SelectedRoleID, osrole.Title"
  ." , IFNULL(osagent.ActiveGroupID, '%s') AS ActiveGroupID"
  ." FROM {osgroup} JOIN {osgroupmembership} ON (osgroup.GroupID = osgroupmembership.GroupID)"
  ."              JOIN {osrole} ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
  ."         LEFT JOIN {osagent} ON (osagent.AgentID = osgroupmembership.AgentID)"
  ." WHERE osgroupmembership.AgentID = '%s'";
  $values = array(UUID_ZERO, $agentID);

  db_set_active('os_groups');
  $groupmembershipResults = db_query($sql, $values);
  if (!$groupmembershipResults) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $groupResults = array();
  $num_rows = 0;
  while ($groupMembershipInfo = db_fetch_array($groupmembershipResults)) {
    ++ $num_rows;
    $groupID = $groupMembershipInfo['GroupID'];
    $sql = " SELECT BIT_OR(osrole.Powers) AS GroupPowers"
    ." FROM {osgrouprolemembership} JOIN osrole ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
    ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";
    $values = array($groupID, $agentID);
    db_set_active('os_groups');
    $groupPowersResult = db_query($sql, $values);
    if (!$groupPowersResult) {
      db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
    $groupPowersInfo = db_fetch_array($groupPowersResult);
    $groupResults[$groupID] = array_merge($groupMembershipInfo, $groupPowersInfo);
  }
  db_set_active('default');

  if ($num_rows == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Memberships',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  return $groupResults;
}

function d4os_io_db_os_groups_services_canAgentViewRoleMembers($params) {
  $sql  = " SELECT CASE WHEN min(OwnerRoleMembership.AgentID) IS NOT NULL THEN 1 ELSE 0 END AS IsOwner ";
  $sql .= " FROM {osgroup} JOIN {osgroupmembership} ON (osgroup.GroupID = osgroupmembership.GroupID AND osgroupmembership.AgentID = '%s')";
  $sql .= "         LEFT JOIN {osgrouprolemembership} AS OwnerRoleMembership ON (OwnerRoleMembership.GroupID = osgroup.GroupID ";
  $sql .= "                   AND OwnerRoleMembership.RoleID  = osgroup.OwnerRoleID ";
  $sql .= "                   AND OwnerRoleMembership.AgentID = '%s')";
  $sql .= " WHERE osgroup.GroupID = '%s' GROUP BY osgroup.GroupID";	
  $values = array($agentID, $agentID, $groupID);

  db_set_active('os_groups');
  $viewMemberResults = db_query($sql, $values);
  if (!$viewMemberResults) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
    );
  }
  $viewMemberInfo = db_fetch_array($viewMemberResults);
  db_set_active('default');
  if (!is_array($viewMemberInfo)) {
    return array(
      'error' => t("Could not define if agent can view role members"),
    );
  }
  return $viewMemberInfo;
}

function d4os_io_db_os_groups_services_getGroupMembers($params) {
  $groupID = $params['GroupID'];
  
  $sql = " SELECT osgroupmembership.AgentID"
  ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
  ." , osgroupmembership.SelectedRoleID, osrole.Title"
  ." , CASE WHEN OwnerRoleMembership.AgentID IS NOT NULL THEN 1 ELSE 0 END AS IsOwner"
  ." FROM {osgroup} JOIN {osgroupmembership} ON (osgroup.GroupID = osgroupmembership.GroupID)"
  ."              JOIN {osrole} ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
  ."              JOIN {osrole} AS OwnerRole ON (osgroup.OwnerRoleID  = OwnerRole.RoleID AND osgroup.GroupID  = OwnerRole.GroupID)"
  ."         LEFT JOIN {osgrouprolemembership} AS OwnerRoleMembership ON (osgroup.OwnerRoleID       = OwnerRoleMembership.RoleID 
                                  AND (osgroup.GroupID           = OwnerRoleMembership.GroupID)
                                  AND (osgroupmembership.AgentID = OwnerRoleMembership.AgentID))"
  ." WHERE osgroup.GroupID = '%s'";
  $values = array($groupID);
  db_set_active('os_groups');
  $groupmemberResults = db_query($sql, $values);
  if (!$groupmemberResults) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $roleMembersVisibleBit = $groupPowers['RoleMembersVisible'];
  $canViewAllGroupRoleMembers = d4os_io_services_groups_canAgentViewRoleMembers($requestingAgent, $groupID, '');
  $members = 0;
  $memberResults = array();
  while ($memberInfo = db_fetch_array($groupmemberResults)) {
    ++ $members;
    $agentID = $memberInfo['AgentID'];
    $sql = " SELECT BIT_OR(osrole.Powers) AS AgentPowers, ( BIT_OR(osrole.Powers) & %d) as MemberVisible"
    ." FROM {osgrouprolemembership} JOIN {osrole} ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
    ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";
    $values = array($roleMembersVisibleBit, $groupID, $agentID);
    db_set_active('os_groups');
    $memberPowersResult = db_query($sql, $values);
    if (!$memberPowersResult) {
      db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }

    $memberPowersInfo = db_fetch_array($memberPowersResult);
    db_set_active('default');

    if (!is_array($memberPowersInfo)) {
      if( $canViewAllGroupRoleMembers || ($memberResults[$agentID] == $requestingAgent)) {
        $memberResults[$agentID] = array_merge($memberInfo, array('AgentPowers' => 0));
      }
      else {
        // if can't view all group role members and there is no Member Visible bit, then don't return this member's info
        unset($memberResults[$agentID]);
      }
    }
    else {
      if ($memberPowersInfo['MemberVisible'] || $canViewAllGroupRoleMembers  || ($memberResults[$agentID] == $requestingAgent)) {
        $memberResults[$agentID] = array_merge($memberInfo, $memberPowersInfo);
      }
      else {
        // if can't view all group role members and there is no Member Visible bit, then don't return this member's info
        unset($memberResults[$agentID]);
      }
    }
  }
  return array('success' => 'true', 'members' => $members, 'member_result' => $memberResults);
}

function d4os_io_db_os_groups_services_getAgentActiveMembership($params) {
  $agentID = $params['AgentID'];
  
  $sql = " SELECT osgroup.GroupID, osgroup.Name as GroupName, osgroup.Charter, osgroup.InsigniaID, osgroup.FounderID, osgroup.MembershipFee, osgroup.OpenEnrollment, osgroup.ShowInList, osgroup.AllowPublish, osgroup.MaturePublish"
  ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
  ." , osgroupmembership.SelectedRoleID, osrole.Title"
  ." , osagent.ActiveGroupID "
  ." FROM {osagent} JOIN {osgroup} ON (osgroup.GroupID = osagent.ActiveGroupID)"
  ."              JOIN {osgroupmembership} ON (osgroup.GroupID = osgroupmembership.GroupID AND osagent.AgentID = osgroupmembership.AgentID)"
  ."              JOIN {osrole} ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
  ." WHERE osagent.AgentID = '%s'";
  $values = array($agentID);
  db_set_active('os_groups');
  $groupmembershipResult = db_query($sql, $values);
  if (!$groupmembershipResult) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $groupMembershipInfo = db_fetch_array($groupmembershipResult);

  if (!is_array($groupMembershipInfo)) {
    db_set_active('default');
    return array(
      'succeed' => 'false',
      'error' => 'No Active Group Specified',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  $groupID = $groupMembershipInfo['GroupID'];
  $sql = " SELECT BIT_OR(osrole.Powers) AS GroupPowers"
  ." FROM {osgrouprolemembership} JOIN {osrole} ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
  ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";
  $values = array($groupID, $agentID);

  $groupPowersResult = db_query($sql, $values);
  if (!$groupPowersResult) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  $groupPowersInfo = db_fetch_array($groupPowersResult);
  db_set_active('default');

  return array_merge($groupMembershipInfo, $groupPowersInfo);
}

function d4os_io_db_os_groups_services_getAgentRoles($params) {
  $agentID = $params['AgentID'];
  
  $sql = " SELECT "
  ." osrole.RoleID, osrole.GroupID, osrole.Title, osrole.Name, osrole.Description, osrole.Powers"
  ." , CASE WHEN osgroupmembership.SelectedRoleID = osrole.RoleID THEN 1 ELSE 0 END AS Selected"
  ." FROM {osgroupmembership} JOIN {osgrouprolemembership}  ON (osgroupmembership.GroupID = osgrouprolemembership.GroupID AND osgroupmembership.AgentID = osgrouprolemembership.AgentID)"
  ."                        JOIN {osrole} ON ( osgrouprolemembership.RoleID = osrole.RoleID AND osgrouprolemembership.GroupID = osrole.GroupID)"
  ."                   LEFT JOIN {osagent} ON (osagent.AgentID = osgroupmembership.AgentID)"
  ." WHERE osgroupmembership.AgentID = '%s'";
  $values = array($agentID);
  
  if (isset($params['GroupID'])) {
    $groupID = $params['GroupID'];
    $sql .= " AND osgroupmembership.GroupID = '%s'";
    $values[] = $groupID;
  }

  db_set_active('os_groups');
  $roleResults = db_query($sql, $values);
  if (!$roleResults) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $roles = array();
  while($role = db_fetch_array($roleResults)) {
    $ID = $role['GroupID'].$role['RoleID'];
    $roles[$ID] = $role;
  }
  db_set_active('default');
  if (!is_array($roles)) {
    return array(
      'error' => t("Could not get agent roles"),
      'params' => var_export($params, TRUE)
    );
  }
  return $roles;
}

function d4os_io_db_os_groups_services_getGroupRoles($params) {
  $groupID = $params['GroupID'];
  
  $sql = " SELECT "
  ." osrole.RoleID, osrole.Name, osrole.Title, osrole.Description, osrole.Powers, count(osgrouprolemembership.AgentID) as Members"
  ." FROM {osrole} LEFT JOIN {osgrouprolemembership} ON (osrole.GroupID = osgrouprolemembership.GroupID AND osrole.RoleID = osgrouprolemembership.RoleID)"
  ." WHERE osrole.GroupID = '%s'"
  ." GROUP BY osrole.RoleID, osrole.Name, osrole.Title, osrole.Description, osrole.Powers";
  $values = array($groupID);

  db_set_active('os_groups');
  $roleResults = db_query($sql, $values);
  if (!$roleResults) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $roles = array();
  while($role = db_fetch_array($roleResults)) {
    $RoleID = $role['RoleID'];
    $roles[$RoleID] = $role;
  }
  db_set_active('default');
  if (!is_array($roles)) {
    return array(
      'error' => t("Could not get group roles"),
      'params' => var_export($params, TRUE)
    );
  }
  return $roles;
}

function d4os_io_db_os_groups_services_getGroupRoleMembers($params) {
  $groupID = $params['GroupID'];

  $roleMembersVisibleBit = $groupPowers['RoleMembersVisible'];
  $canViewAllGroupRoleMembers = d4os_io_services_groups_canAgentViewRoleMembers($requestingAgent, $groupID, '');
  
  $sql = " SELECT "
  ." osrole.RoleID, osgrouprolemembership.AgentID"
  ." , (osrole.Powers & %d) as MemberVisible"
  ." FROM {osrole} JOIN {osgrouprolemembership} ON (osrole.GroupID = osgrouprolemembership.GroupID AND osrole.RoleID = osgrouprolemembership.RoleID)"
  ." WHERE osrole.GroupID = '%s'";
  $values = array($roleMembersVisibleBit, $groupID);

  db_set_active('os_groups');
  $memberResults = db_query($sql, $values);
  if (!$memberResults) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
	
  $members = array();
  $membersCount = 0;
  while($member = db_fetch_array($memberResults)) {
    ++ $membersCount;
    if ($canViewAllGroupRoleMembers || $member['MemberVisible'] || ($member['AgentID'] == $requestingAgent)) {
      $Key = $member['AgentID'] . $member['RoleID'];
      $members[$Key ] = $member;
    }
  }
  db_set_active('default');
  if (!is_array($members)) {
    return array(
      'error' => t("Could not get group role members"),
      'params' => var_export($params, TRUE)
    );
  }
  return array('success' => 'true', 'members' => $members, 'members_count' => $membersCount);
}

function d4os_io_db_os_groups_services_setAgentGroupInfo($params) {
  if (isset($params['AgentID'])) {
    $agentID = $params['AgentID'];
  }
  else {
    $agentID = "";
  }
  if (isset($params['GroupID'])) {
    $groupID = $params['GroupID'];
  }
  else {
    $groupID = "";
  }
  if (isset($params['SelectedRoleID'])) {
    $roleID  = $params['SelectedRoleID'];
  }
  else {
    $roleID = "";
  }
  if (isset($params['AcceptNotices'])) {
    $acceptNotices  = $params['AcceptNotices'];
  }
  else {
    $acceptNotices = 1;
  }
  if (isset($params['ListInProfile'])) {
    $listInProfile  = $params['ListInProfile'];
  }
  else {
    $listInProfile = 0;
  }
  
  if (isset($requestingAgent) && ($requestingAgent != UUID_ZERO) && ($requestingAgent != $agentID)) {
    return array(
      'error' => "Agent can only change their own group info",
      'params' => var_export($params, TRUE)
    );
  }

  $sql = " UPDATE "
  ."     {osgroupmembership}"
  ." SET "
  ."    AgentID = '%s'";
  $values = array($agentID);
  
  if (isset($params['SelectedRoleID'])) {
    $sql .="    , SelectedRoleID = '%s'";
    $values[] = $roleID;
  }
  if (isset($params['AcceptNotices'])) {
    $sql .="    , AcceptNotices = %d";
    $values[] = $acceptNotices;
  }
  if (isset($params['ListInProfile'])) {
    $sql .="    , ListInProfile = %d";
    $values[] = $listInProfile;
  }
  
  $sql .=" WHERE osgroupmembership.GroupID = '%s' AND osgroupmembership.AgentID = '%s'";
  $values[] = $groupID;
  $values[] = $agentID;

  db_set_active('os_groups');
  $memberResults = db_query($sql, $values);
  db_set_active('default');
  if (!$memberResults) {
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  return array('success'=> 'true');
}

function d4os_io_db_os_groups_services_getGroupNotices($params) {
  $groupID = $params['GroupID'];

  $sql = " SELECT "
  ." GroupID, NoticeID, Timestamp, FromName, Subject, Message, BinaryBucket"
  ." FROM {osgroupnotice}"
  ." WHERE osgroupnotice.GroupID = '$groupID'";
  $values = array($groupID);

  db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $notices = array();
  while ($notice = db_fetch_array($results)) {
    $NoticeID = $notice['NoticeID'];
    $notices[$NoticeID] = $notice;
  }
  db_set_active('default');
  if (!is_array($notices)) {
    return array(
      'error' => t("Could not get group notices"),
      'params' => var_export($params, TRUE)
    );
  }
  return $notices;
}

function d4os_io_db_os_groups_services_getGroupNotice($params) {
  $noticeID = $params['NoticeID'];

  $sql = " SELECT "
  ." GroupID, NoticeID, Timestamp, FromName, Subject, Message, BinaryBucket"
  ." FROM {osgroupnotice}"
  ." WHERE osgroupnotice.NoticeID = '%s'";
  $values = array($noticeID);

  db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $notice = db_fetch_array($results);
  db_set_active('default');
  if (!is_array($notice)) {
    return array(
      'error' => t("Could not get group notice"),
      'params' => var_export($params, TRUE)
    );
  }
  return $notice;
}

function d4os_io_db_os_groups_services_addGroupNotice($params) {
  $groupID  = $params['GroupID'];
  $noticeID = $params['NoticeID'];
  $fromName = addslashes($params['FromName']);
  $subject  = addslashes($params['Subject']);
  $binaryBucket = $params['BinaryBucket'];
  $message      = addslashes($params['Message']);
  $timeStamp    = $params['TimeStamp'];
  $sql = " INSERT INTO {osgroupnotice}"
  ." (GroupID, NoticeID, Timestamp, FromName, Subject, Message, BinaryBucket)"
  ." VALUES "
  ." ('%s', '%s', %d, '%s', '%s', '%s', '%s')";
  $values = array($groupID, $noticeID, $timeStamp, $fromName, $subject, $message, $binaryBucket);

  db_set_active('os_groups');
  $results = db_query($sql, $values);
  db_set_active('default');
  if (!$results) {
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_addAgentToGroupInvite($params) {
  $inviteID = $params['InviteID'];
  $groupID = $params['GroupID'];
  $roleID  = $params['RoleID'];
  $agentID = $params['AgentID'];
  // Remove any existing invites for this agent to this group
  $sql = " DELETE FROM {osgroupinvite}"
  ." WHERE osgroupinvite.AgentID = '%s' AND osgroupinvite.GroupID = '%s'";
  $values = array($agentID, $groupID);

  db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  
  // Add new invite for this agent to this group for the specifide role
  $sql = " INSERT INTO {osgroupinvite}"
  ." (InviteID, GroupID, RoleID, AgentID) VALUES ('%s', '%s', '%s', '%s')";
  $values = array($inviteID, $groupID, $roleID, $agentID);
  
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  db_set_active('default');
  
  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_getAgentToGroupInvite($params) {
  $inviteID = $params['InviteID'];

  $sql = " SELECT GroupID, RoleID, AgentID FROM {osgroupinvite}"
  ." WHERE osgroupinvite.InviteID = '%s'";
  $values = array($inviteID);

  db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $inviteInfo = db_fetch_array($results);
  db_set_active('default');

  $return = array();
  if (!is_array($inviteInfo)) {
    $groupID  = $inviteInfo['GroupID'];
    $roleID   = $inviteInfo['RoleID'];
    $agentID  = $inviteInfo['AgentID'];
    $return = array(
      'success' => 'true',
      'GroupID'=>$groupID,
      'RoleID'=>$roleID,
      'AgentID'=>$agentID
    );
  }
  else {
    $return = array(
      'succeed' => 'false',
      'error' => 'Invitation not found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  return $return;
}

function d4os_io_db_os_groups_services_removeAgentToGroupInvite($params) {
  $inviteID = $params['InviteID'];

  $sql = " DELETE FROM {osgroupinvite}"
  ." WHERE osgroupinvite.InviteID = '%s'";
  $values = array($inviteID);

  db_set_active('os_groups');
  $results = db_query($sql, $values);
  db_set_active('default');
  if (!$results) {
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_checkGroupPermission($PermMask, $PermValue) {
  $sql = " SELECT %d & %d AS Allowed";
  $values = array($PermMask, $PermValue);
  db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    db_set_active('default');
    echo print_r( array(
      'error' => "Could not successfully run query ($sql) from DB: " . mysql_error())
    );
  }
  $PermMasked = db_result($results, 0);
  db_set_active('default');
  return $PermMasked;
}