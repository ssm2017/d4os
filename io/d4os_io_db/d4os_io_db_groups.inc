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
  d4os_io_db_set_active('os_groups');
  $result = db_query('SELECT *, Name AS title FROM {osgroup}');
  while ($group = db_fetch_array($result)) {
    $groups[] = $group;
  }
  d4os_io_db_set_active('default');
  return $groups;
}

function d4os_io_db_os_group_delete($params) {
  // get the group
  $group = d4os_io_db_os_group_load($params);
  db_query("DELETE FROM {d4os_ui_groups} WHERE nid = %d". $group['nid']);
  d4os_io_db_set_active('os_groups');
  db_query("DELETE FROM {osagent} WHERE ActiveGroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroupinvite} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroupmembership} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroupnotice} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgrouprolemembership} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osrole} WHERE GroupID = '%s'", array($group['GroupID']));
  db_query("DELETE FROM {osgroup} WHERE GroupID = '%s'", array($group['GroupID']));
  d4os_io_db_set_active('default');
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
 *
 * The code in this script was inspired and mostly copied from flotsam xmlrpc groups implementation
 * http://code.google.com/p/flotsam/wiki/XmlRpcGroups
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
  $sql = "INSERT INTO {osgroup} ( "
        ."GroupID, "        // 1
        ."Name, "           // 2
        ."Charter, "        // 3
        ."InsigniaID, "     // 4
        ."FounderID, "      // 5
        ."MembershipFee, "  // 6
        ."OpenEnrollment, " // 7
        ."ShowInList, "     // 8
        ."AllowPublish, "   // 9
        ."MaturePublish, "  // 10
        ."OwnerRoleID "     // 11
        .") VALUES ("
        ."'%s', " // 1 GroupID
        ."'%s', " // 2 Name
        ."'%s', " // 3 Charter
        ."'%s', " // 4 InsigniaID
        ."'%s', " // 5 FounderID
        ."%d, "   // 6 MembershipFee
        ."%d, "   // 7 OpenEnrollment
        ."%d, "   // 8 ShowInList
        ."%d, "   // 9 AllowPublish
        ."%d, "   // 10 MaturePublish
        ."'%s'"   // 11 OwnerRoleID
        .")";
  $values = array($groupID, $name, $charter, $insigniaID, $founderID, $membershipFee, $openEnrollment, $showInList, $allowPublish, $maturePublish, $ownerRoleID);

  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');

  // Create Everyone Role
  // NOTE: FIXME: This is a temp fix until the libomv enum for group powers is fixed in OpenSim
  $params = array(
    'GroupID'     => $groupID,
    'RoleID'      => UUID_ZERO,
    'Name'        => 'Everyone',
    'Description' => t('Everyone in the group is in the everyone role.'),
    'Title'       => "Member of $name",
    'Powers'      => $everyonePowers
  );
  $result = _d4os_io_services_groups_addRoleToGroup($params);
  if (isset($result['error'])) {
    return $result;
  }

  // Create Owner Role
  $params = array(
    'GroupID'     => $groupID,
    'RoleID'      => $ownerRoleID,
    'Name'        => 'Owners',
    'Description' => t('Owners of !groupname', array('!groupname' => $name)),
    'Title'       => t('Owner of !groupname', array('!groupname' => $name)),
    'Powers'      => $ownersPowers
  );
  $result = _d4os_io_services_groups_addRoleToGroup($params);
  if (isset($result['error'])) {
    return $result;
  }

  // Add founder to group, will automatically place them in the Everyone Role, also places them in specified Owner Role
  $params = array(
    'AgentID' => $founderID,
    'GroupID' => $groupID,
    'RoleID'  => $ownerRoleID
  );
  $result = _d4os_io_services_groups_addAgentToGroup($params);
  if (isset($result['error'])) {
    return $result;
  }

  // Select the owner's role for the founder
  $params = array(
    'AgentID' => $founderID,
    'RoleID'  => $ownerRoleID,
    'GroupID' => $groupID
  );
  $result = _d4os_io_services_groups_setAgentGroupSelectedRole($params);
  if (isset($result['error'])) {
    return $result;
  }

  // Set the new group as the founder's active group
  $params = array(
    'AgentID' => $founderID,
    'GroupID' => $groupID
  );
  $result = _d4os_io_services_groups_setAgentActiveGroup($params);
  if (isset($result['error'])) {
    return $result;
  }

  // reload the created group
  $params = array(
    "GroupID"=>$groupID
  );
  return d4os_io_services_groups_getGroup($params);
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

  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $data)) {
    d4os_io_db_set_active('default');
    return array(
      'error'   => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'method'  => 'd4os_io_services_groups_addRoleToGroup',
      'params'  => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');

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

  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');

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
  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $sql = "UPDATE {osgroupmembership} SET SelectedRoleID = '%s' WHERE GroupID = '%s' AND SelectedRoleID = '%s'";
  $values = array(UUID_ZERO, $groupID, $roleID);
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');

  $sql = "DELETE FROM {osrole} WHERE GroupID = '$groupID' AND RoleID = '$roleID'";
  $values = array($groupID, $roleID);
  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');
  return array("success" => "true");
}

function d4os_io_db_os_groups_services_getGroup($params) {
  $sql = " SELECT osgroup.GroupID, osgroup.Name, Charter, InsigniaID, FounderID, MembershipFee, OpenEnrollment, ShowInList, AllowPublish, MaturePublish, OwnerRoleID"
        ." , count(osrole.RoleID) as GroupRolesCount, count(osgroupmembership.AgentID) as GroupMembershipCount "
        ." FROM {osgroup} AS osgroup"
        ." LEFT JOIN {osrole} AS osrole ON (osgroup.GroupID = osrole.GroupID)"
        ." LEFT JOIN {osgroupmembership} AS osgroupmembership ON (osgroup.GroupID = osgroupmembership.GroupID)"
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

  d4os_io_db_set_active('os_groups');
  $result = db_query($sql, $values);
  d4os_io_db_set_active('default');

  if (!$result) {
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  d4os_io_db_set_active('os_groups');
  $group = db_fetch_array($result);
  d4os_io_db_set_active('default');
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
  $sql = "UPDATE {osgroup} SET "
        ."Name = '%s', "
        ."Charter = '%s', "
        ."InsigniaID = '%s', "
        ."MembershipFee = %d, "
        ."OpenEnrollment= %d, "
        ."ShowInList    = %d, "
        ."AllowPublish  = %d, "
        ."MaturePublish = %d "
        ."WHERE GroupID = '%s'";
  $values = array($name, $charter, $insigniaID, $membershipFee, $openEnrollment, $showInList, $allowPublish, $maturePublish, $groupID);

  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');

  if ($inworld) {
    module_invoke_all('os_group_grid_update', $params);
  }

  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_findGroups($params) {
  $search = addslashes($params['Search']);

  $sql = " SELECT osgroup.GroupID, osgroup.Name, count(osgroupmembership.AgentID) as Members "
        ." FROM {osgroup} AS osgroup"
        ." LEFT JOIN {osgroupmembership} AS osgroupmembership ON (osgroup.GroupID = osgroupmembership.GroupID) "
        ." WHERE "
        ." ( MATCH (osgroup.name) AGAINST ('%s' IN BOOLEAN MODE)"
        ."   OR osgroup.name LIKE '%%s%'"
        ."   OR osgroup.name REGEXP '%s'"
        ." ) AND ShowInList = 1" 
        ." GROUP BY osgroup.GroupID, osgroup.Name";
  $values = array($search, $search, $search);

  d4os_io_db_set_active('os_groups');
  $result = db_query($sql, $values);

  if (!$result) {
    d4os_io_db_set_active('default');
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
  d4os_io_db_set_active('default');

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
  global $requestingAgent;

  $agentID = $params['AgentID'];
  $groupID = $params['GroupID'];

  if (isset($requestingAgent) && ($requestingAgent != UUID_ZERO) && ($requestingAgent != $agentID)) {
    return array(
      'error'   => t('Agent can only change their own Selected Group Role'),
      'params'  => var_export($params, TRUE)
    );
  }

  $sql = " UPDATE {osagent} "
        ." SET ActiveGroupID = '%s'"
        ." WHERE AgentID = '%s'";
  $values = array($groupID, $agentID);
  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  if( db_affected_rows() == 0 ) {
    $sql = " INSERT INTO {osagent} (ActiveGroupID, AgentID) VALUES ('%s', '%s')";
  
    if (!db_query($sql, $values)) {
      d4os_io_db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
  }
  d4os_io_db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_addAgentToGroup($params) {
  global $groupPowers;

  $groupID = $params["GroupID"];
  $agentID = $params["AgentID"];

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['AssignMember']))) {
    // If they don't have direct permission, check to see if the group is marked for open enrollment
    $groupInfo = _d4os_io_services_groups_getGroup(array ('GroupID'=>$groupID));
    if( isset($groupInfo['error'])) {
      return $groupInfo;
    }

    if($groupInfo['OpenEnrollment'] != 1) {

      // Group is not open enrollment, check if the specified agentid has an invite
      $sql = " SELECT GroupID, RoleID, AgentID"
            ." FROM {osgroupinvite}"
            ." WHERE AgentID = '%s' AND GroupID = '%s'";
      $values = array($agentID, $groupID);
    
      d4os_io_db_set_active('os_groups');
      $results = db_query($sql, $values);
      if (!$results) {
        d4os_io_db_set_active('default');
        return array(
          'error'   => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
          'params'  => var_export($params, TRUE)
        );
      }

      $inviteInfo = NULL;
      while ($temp = db_fetch_array($results)) {
        $inviteInfo = $temp;
      }
      d4os_io_db_set_active('default');
      if (!is_array($inviteInfo)) {
        return array(
          'error'   => t("Could not add agent to group "),
          'params'  => var_export($params, TRUE)
        );
      }
      return $inviteInfo;
  
      if(is_array($inviteInfo) && isset($inviteInfo['RoleID'])) {
        // if there is an invite, make sure we're adding the user to the role specified in the invite
        $params['RoleID'] = $inviteInfo['RoleID'];
      }
      else {
        // Not openenrollment, not invited, return permission denied error
        return $error;
      }
    }
  }
  return _d4os_io_services_groups_addAgentToGroup($params);
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
  d4os_io_db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  $members_count = db_result($result);

  // If not a member, add membership, select role (defaults to uuidZero, or everyone role)
  if ($members_count == 0) {
    $sql = " INSERT INTO {osgroupmembership} "
          ."(GroupID, AgentID, Contribution, ListInProfile, AcceptNotices, SelectedRoleID)"
          ." VALUES "
          ."('%s','%s', 0, 1, 1,'%s')";
    $values = array($groupID, $agentID, $roleID);

    if (!db_query($sql, $values)) {
      d4os_io_db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
  }
  d4os_io_db_set_active('default');

  // Make sure they're in the Everyone role
  $params = array(
    "GroupID" => $groupID,
    "RoleID"  => UUID_ZERO,
    "AgentID" => $agentID
  );
  $result = _d4os_io_services_groups_addAgentToGroupRole($params);
  if(isset($result['error'])) {
    return $result;
  }

  // Make sure they're in specified role, if they were invited
  if ($roleID != UUID_ZERO) {
    $params = array(
      "GroupID" => $groupID,
      "RoleID"  => $roleID,
      "AgentID" => $agentID
    );
    $result = _d4os_io_services_groups_addAgentToGroupRole($params);
    if(isset($result['error'])) {
      return $result;
    }
  }

  //Set the role they were invited to as their selected role
  $params = array(
    'AgentID' => $agentID,
    'RoleID'  => $roleID,
    'GroupID' => $groupID
  );
  _d4os_io_services_groups_setAgentGroupSelectedRole($params);
  
  // Set the group as their active group.
  // _d4os_io_services_groups_setAgentActiveGroup(array("GroupID" => $groupID, "AgentID" => $agentID));
  return array("success" => "true");
}

function d4os_io_db_os_groups_services_removeAgentFromGroup($params) {
  global $requestingAgent, $groupPowers;

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];

  // An agent is always allowed to remove themselves from a group -- so only check if the requesting agent is different then the agent being removed.
  if ($agentID != $requestingAgent) {
    if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['RemoveMember']))) {
      return $error;
    }
  }

  // 1. If group is agent's active group, change active group to uuidZero
  // 2. Remove Agent from group (osgroupmembership)
  // 3. Remove Agent from all of the groups roles (osgrouprolemembership)

  $sql = " UPDATE {osagent} "
        ." SET ActiveGroupID = 'UUID_ZERO'"
        ." WHERE AgentID = '%s' AND ActiveGroupID = '%s'";
  $values = array($agentID, $groupID);
  d4os_io_db_set_active('os_groups');
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $sql = " DELETE FROM {osgroupmembership} "
        ." WHERE AgentID = '%s' AND GroupID = '%s'";
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $sql = " DELETE FROM {osgrouprolemembership} "
        ." WHERE AgentID = '%s' AND GroupID = '%s'";
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');
  return array("success" => "true");
}

function d4os_io_db_os_groups_services_addAgentToGroupRole_2($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];

  // Check if agent already a member
  $sql = " SELECT count(AgentID) as isMember FROM {osgrouprolemembership} WHERE AgentID = '%s' AND RoleID = '%s' AND GroupID = '%s'";
  $values = array($agentID, $roleID, $groupID);
  d4os_io_db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  if (db_result($result, 0) == 0) {
    $sql = " INSERT INTO {osgrouprolemembership} (AgentID, RoleID, GroupID) VALUES ('%s', '%s', '%s')";
    
    if (!db_query($sql, $values)) {
      d4os_io_db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
  }
  d4os_io_db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_addAgentToGroupRole($params) {
  global $requestingAgent, $groupPowers;

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];
  
  // Check if being assigned to Owners role, assignments to an owners role can only be requested by owners.
  $sql = " SELECT OwnerRoleID, osgrouprolemembership.AgentID "
        ." FROM {osgroup} AS osgroup"
        ." LEFT JOIN {osgrouprolemembership} AS osgrouprolemembership ON (osgroup.GroupID = osgrouprolemembership.GroupID AND osgroup.OwnerRoleID = osgrouprolemembership.RoleID) "
        ." WHERE osgrouprolemembership.AgentID = '%s' AND osgroup.GroupID = '%s'";
  $values = array($requestingAgent, $groupID);
  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  $ownerRoleInfo = db_fetch_array($results);
  d4os_io_db_set_active('default');
  if (!is_array($ownerRoleInfo)) {
    return array(
      'error' => t("Could not add agent to group role"),
      'params' => var_export($params, TRUE)
    );
  }

  if(!is_array($ownerRoleInfo) || !isset($ownerRoleInfo['OwnerRoleID'])) {
    return array(
      'error' => t("Group (!group) not found or Agent (!agent) is not in the owner's role", array('!group' => $groupID, '!agent' => $agentID)),
      'params' => var_export($params, TRUE)
    );
  }

  if (($ownerRoleInfo['OwnerRoleID'] == $roleID) && ($ownerRoleInfo['AgentID'] != $requestingAgent)) {
    return array(
      'error' => t("Requesting agent !requestingAgent is not a member of the Owners Role and cannot add members to the owners role.", array('!requestingAgent' => $requestingAgent)),
      'params' => var_export($params, TRUE)
    );
  }

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['AssignMember']))) {
    return $error;
  }

  return _d4os_io_services_groups_addAgentToGroupRole($params);
}

function d4os_io_db_os_groups_services_removeAgentFromGroupRole($params) {
  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID  = $params["RoleID"];
  // If agent has this role selected, change their selection to everyone (uuidZero) role
  $sql = " UPDATE {osgroupmembership} SET SelectedRoleID = '%s' WHERE AgentID = '%s' AND GroupID = '%s' AND SelectedRoleID = '%s'";
  $values = array(UUID_ZERO, $agentID, $groupID, $roleID);
  d4os_io_db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  
  $sql = " DELETE FROM {osgrouprolemembership} WHERE AgentID = '%s' AND GroupID = '%s' AND RoleID = '%s'";
  $values = array($agentID, $groupID, $roleID);
  if (!db_query($sql, $values)) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');

  return array("success" => "true");
}

function d4os_io_db_os_groups_services_setAgentGroupSelectedRole($params) {
  global $requestingAgent;

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];

  if (isset($requestingAgent) && ($requestingAgent != UUID_ZERO) && ($requestingAgent != $agentID)) {
    return array(
      'error'   => t('Agent can only change their own Selected Group Role'),
      'params'  => var_export($params, TRUE)
    );
  }

  $sql = " UPDATE {osgroupmembership} SET SelectedRoleID = '%s' WHERE AgentID = '%s' AND GroupID = '%s'";
  $values = array($roleID, $agentID, $groupID);
  d4os_io_db_set_active('os_groups');
  $result = db_query($sql, $values);
  if (!$result) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');

  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_getAgentGroupMembership($params) {
  $groupID = $params['GroupID'];
  $agentID = $params['AgentID'];

  $sql = " SELECT osgroup.GroupID, osgroup.Name as GroupName, osgroup.Charter, osgroup.InsigniaID, osgroup.FounderID, osgroup.MembershipFee, osgroup.OpenEnrollment, osgroup.ShowInList, osgroup.AllowPublish, osgroup.MaturePublish"
        ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
        ." , osgroupmembership.SelectedRoleID, osrole.Title"
        ." , osagent.ActiveGroupID "
        ." FROM {osgroup} AS osgroup"
        ." JOIN {osgroupmembership} AS osgroupmembership ON (osgroup.GroupID = osgroupmembership.GroupID)"
        ." JOIN {osrole} AS osrole ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
        ." JOIN {osagent} AS osagent ON (osagent.AgentID = osgroupmembership.AgentID)"
        ." WHERE osgroup.GroupID = '%s' AND osgroupmembership.AgentID = '%s'";
  $values = array($groupID, $agentID);

  d4os_io_db_set_active('os_groups');
  $groupmembershipResult = db_query($sql, $values);
  if (!$groupmembershipResult) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $groupMembershipInfo = array();
  while($temp = db_fetch_array($groupmembershipResult)) {
    $groupMembershipInfo[] = $temp;
  }
  d4os_io_db_set_active('default');

  if (count($groupMembershipInfo) == 0) {
    return array(
      'succeed' => 'false',
      'error'   => 'None Found',
      'params'  => var_export($groupMembershipInfo, TRUE),//var_export($params, TRUE),
      'sql'     => $sql
    );
  }

  $sql = " SELECT BIT_OR(osrole.Powers) AS GroupPowers"
        ." FROM {osgrouprolemembership} AS osgrouprolemembership"
        ." JOIN {osrole} AS osrole ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
        ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";

  d4os_io_db_set_active('os_groups');
  $groupPowersResult = db_query($sql, $values);
  if (!$groupPowersResult) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $groupPowersInfo = array();
  while($temp = db_fetch_array($groupPowersResult)) {
    $groupPowersInfo[] = $temp;
  }
  d4os_io_db_set_active('default');

  return array_merge($groupMembershipInfo[0], $groupPowersInfo[0]);
}

function d4os_io_db_os_groups_services_getAgentGroupMemberships($params) {
  $agentID = $params['AgentID'];
  
  $sql = " SELECT osgroup.GroupID, osgroup.Name as GroupName, osgroup.Charter, osgroup.InsigniaID, osgroup.FounderID, osgroup.MembershipFee, osgroup.OpenEnrollment, osgroup.ShowInList, osgroup.AllowPublish, osgroup.MaturePublish"
        ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
        ." , osgroupmembership.SelectedRoleID, osrole.Title"
        ." , IFNULL(osagent.ActiveGroupID, '%s') AS ActiveGroupID"
        ." FROM {osgroup} AS osgroup"
        ." JOIN {osgroupmembership} AS osgroupmembership ON (osgroup.GroupID = osgroupmembership.GroupID)"
        ." JOIN {osrole} AS osrole ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
        ." LEFT JOIN {osagent} AS osagent ON (osagent.AgentID = osgroupmembership.AgentID)"
        ." WHERE osgroupmembership.AgentID = '%s'";
  $values = array(UUID_ZERO, $agentID);

  d4os_io_db_set_active('os_groups');
  $groupmembershipResults = db_query($sql, $values);
  if (!$groupmembershipResults) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $groupResults = array();
  while ($groupMembershipInfo = db_fetch_array($groupmembershipResults)) {
    $groupID = $groupMembershipInfo['GroupID'];
    $sql = " SELECT BIT_OR(osrole.Powers) AS GroupPowers"
          ." FROM {osgrouprolemembership} AS osgrouprolemembership"
          ." JOIN {osrole} AS osrole ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
          ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";
    $values = array($groupID, $agentID);

    //d4os_io_db_set_active('os_groups');
    $groupPowersResult = db_query($sql, $values);
    if (!$groupPowersResult) {
      d4os_io_db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
    while ($temp = db_fetch_array($groupPowersResult)) {
      $groupPowersInfo = $temp;
    }

    $groupResults[$groupID] = array_merge($groupMembershipInfo, $groupPowersInfo);
  }
  d4os_io_db_set_active('default');

  if (count($groupResults) == 0) {
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
  $agentID  = $params['agentID'];
  $groupID  = $params['groupID'];
  $roleID   = $params['roleID'];
  $membersVisibleTo = variable_get('d4os_io_services_groups_membersVisibleTo', 'Group');

  if( $membersVisibleTo == 'All' ) return TRUE;

  $sql  = " SELECT CASE WHEN min(OwnerRoleMembership.AgentID) IS NOT NULL THEN 1 ELSE 0 END AS IsOwner "
        ." FROM {osgroup} AS osgroup"
        ." JOIN {osgroupmembership} AS osgroupmembership ON (osgroup.GroupID = osgroupmembership.GroupID AND osgroupmembership.AgentID = '%s')"
        ." LEFT JOIN {osgrouprolemembership} AS OwnerRoleMembership ON (OwnerRoleMembership.GroupID = osgroup.GroupID "
        ." AND OwnerRoleMembership.RoleID  = osgroup.OwnerRoleID "
        ." AND OwnerRoleMembership.AgentID = '%s')"
        ." WHERE osgroup.GroupID = '%s' GROUP BY osgroup.GroupID";
  $values = array($agentID, $agentID, $groupID);

  d4os_io_db_set_active('os_groups');
  $viewMemberResults = db_query($sql, $values);
  if (!$viewMemberResults) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
    );
  }

  while ($temp = db_fetch_array($viewMemberResults)) {
    $viewMemberInfo = $temp;
  }
  d4os_io_db_set_active('default');

  if (!is_array($viewMemberInfo)) {
    return FALSE;
  }

  $return = FALSE;
  switch ($membersVisibleTo) {
    case 'Group':
      // if we get to here, there is at least one row, so they are a member of the group
      $return = TRUE;
    case 'Owners':
    default:
      $return = $viewMemberInfo['IsOwner'];			
  }

  return $return;
}

function d4os_io_db_os_groups_services_getGroupMembers($params) {
  global $groupPowers;

  $groupID                = $params['GroupID'];
  $requestingAgent        = $params['RequestingAgentID'];
  $roleMembersVisibleBit  = $groupPowers['RoleMembersVisible'];
  $memberResults          = array();
  $members_infos          = array();
  
  $sql = " SELECT osgroupmembership.AgentID"
        ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
        ." , osgroupmembership.SelectedRoleID, osrole.Title"
        ." , CASE WHEN OwnerRoleMembership.AgentID IS NOT NULL THEN 1 ELSE 0 END AS IsOwner"
        ." FROM {osgroup} AS osgroup"
        ." JOIN {osgroupmembership} AS osgroupmembership ON (osgroup.GroupID = osgroupmembership.GroupID)"
        ." JOIN {osrole} AS osrole ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
        ." JOIN {osrole} AS OwnerRole ON (osgroup.OwnerRoleID = OwnerRole.RoleID AND osgroup.GroupID = OwnerRole.GroupID)"
        ." LEFT JOIN {osgrouprolemembership} AS OwnerRoleMembership ON (osgroup.OwnerRoleID = OwnerRoleMembership.RoleID"
        ." AND (osgroup.GroupID = OwnerRoleMembership.GroupID)"
        ." AND (osgroupmembership.AgentID = OwnerRoleMembership.AgentID))"
        ." WHERE osgroup.GroupID = '%s'";
  $values = array($groupID);

  d4os_io_db_set_active('os_groups');
  $groupmemberResults = db_query($sql, $values);
  if (!$groupmemberResults) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  // get group members
  while ($memberInfo = db_fetch_array($groupmemberResults)) {
    $members_infos[] = $memberInfo;
  }
  d4os_io_db_set_active('default');

  if (count($members_infos) == 0) {
    return array(
      'succeed' => 'false',
      'error' => t('No Group Members found'),
      'sql' => $sql
    );
  }

  // get members rights
  foreach ($members_infos as $memberInfo) {
    $agentID = $memberInfo['AgentID'];
    $sql = " SELECT BIT_OR(osrole.Powers) AS AgentPowers, ( BIT_OR(osrole.Powers) & %d) as MemberVisible"
          ." FROM {osgrouprolemembership} AS osgrouprolemembership"
          ." JOIN {osrole} AS osrole ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
          ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";
    $values = array($roleMembersVisibleBit, $groupID, $agentID);

    d4os_io_db_set_active('os_groups');
    $memberPowersResult = db_query($sql, $values);
    if (!$memberPowersResult) {
      d4os_io_db_set_active('default');
      return array(
        'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
        'params' => var_export($params, TRUE)
      );
    }
    $memberPowersInfo = '';
    while($temp = db_fetch_array($memberPowersResult)) {
      $memberPowersInfo = $temp;
    }
    d4os_io_db_set_active('default');

    $canViewAllGroupRoleMembers = d4os_io_services_groups_canAgentViewRoleMembers(
      array(
        'agentID' => $requestingAgent,
        'groupID' => $groupID,
        'roleID'  => ''
      )
    );

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

  if (count($memberResults) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Visible Group Members found',
      'sql' => $sql
    );
  }

  return $memberResults;
}

function d4os_io_db_os_groups_services_getAgentActiveMembership($params) {
  $agentID = $params['AgentID'];
  
  $sql = " SELECT osgroup.GroupID, osgroup.Name as GroupName, osgroup.Charter, osgroup.InsigniaID, osgroup.FounderID, osgroup.MembershipFee, osgroup.OpenEnrollment, osgroup.ShowInList, osgroup.AllowPublish, osgroup.MaturePublish"
        ." , osgroupmembership.Contribution, osgroupmembership.ListInProfile, osgroupmembership.AcceptNotices"
        ." , osgroupmembership.SelectedRoleID, osrole.Title"
        ." , osagent.ActiveGroupID "
        ." FROM {osagent} AS osagent"
        ." JOIN {osgroup} AS osgroup ON (osgroup.GroupID = osagent.ActiveGroupID)"
        ." JOIN {osgroupmembership} AS osgroupmembership ON (osgroup.GroupID = osgroupmembership.GroupID AND osagent.AgentID = osgroupmembership.AgentID)"
        ." JOIN {osrole} AS osrole ON (osgroupmembership.SelectedRoleID = osrole.RoleID AND osgroupmembership.GroupID = osrole.GroupID)"
        ." WHERE osagent.AgentID = '%s'";
  $values = array($agentID);
  d4os_io_db_set_active('os_groups');
  $groupmembershipResult = db_query($sql, $values);
  if (!$groupmembershipResult) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $groupMembershipInfo = db_fetch_array($groupmembershipResult);

  if (!is_array($groupMembershipInfo)) {
    d4os_io_db_set_active('default');
    return array(
      'succeed' => 'false',
      'error' => 'No Active Group Specified',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  $groupID = $groupMembershipInfo['GroupID'];
  $sql = " SELECT BIT_OR(osrole.Powers) AS GroupPowers"
        ." FROM {osgrouprolemembership} AS osgrouprolemembership"
        ." JOIN {osrole} AS osrole ON (osgrouprolemembership.GroupID = osrole.GroupID AND osgrouprolemembership.RoleID = osrole.RoleID)"
        ." WHERE osgrouprolemembership.GroupID = '%s' AND osgrouprolemembership.AgentID = '%s'";
  $values = array($groupID, $agentID);

  $groupPowersResult = db_query($sql, $values);
  if (!$groupPowersResult) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  $groupPowersInfo = db_fetch_array($groupPowersResult);
  d4os_io_db_set_active('default');

  return array_merge($groupMembershipInfo, $groupPowersInfo);
}

function d4os_io_db_os_groups_services_getAgentRoles($params) {
  $agentID = $params['AgentID'];
  
  $sql = " SELECT osrole.RoleID, osrole.GroupID, osrole.Title, osrole.Name, osrole.Description, osrole.Powers"
        ." , CASE WHEN osgroupmembership.SelectedRoleID = osrole.RoleID THEN 1 ELSE 0 END AS Selected"
        ." FROM {osgroupmembership} AS osgroupmembership"
        ." JOIN {osgrouprolemembership} AS osgrouprolemembership ON (osgroupmembership.GroupID = osgrouprolemembership.GroupID AND osgroupmembership.AgentID = osgrouprolemembership.AgentID)"
        ." JOIN {osrole} AS osrole ON ( osgrouprolemembership.RoleID = osrole.RoleID AND osgrouprolemembership.GroupID = osrole.GroupID)"
        ." LEFT JOIN {osagent} AS osagent ON (osagent.AgentID = osgroupmembership.AgentID)"
        ." WHERE osgroupmembership.AgentID = '%s'";
  $values = array($agentID);
  
  if (isset($params['GroupID'])) {
    $groupID = $params['GroupID'];
    $sql .= " AND osgroupmembership.GroupID = '%s'";
    $values[] = $groupID;
  }

  d4os_io_db_set_active('os_groups');
  $roleResults = db_query($sql, $values);
  if (!$roleResults) {
    d4os_io_db_set_active('default');
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
  d4os_io_db_set_active('default');
  if (!is_array($roles)) {
    return array(
      'error' => t("Could not get agent roles"),
      'params' => var_export($params, TRUE)
    );
  }

  if (count($roles) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'None found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  return $roles;
}

function d4os_io_db_os_groups_services_getGroupRoles($params) {
  $groupID = $params['GroupID'];
  
  $sql = " SELECT osrole.RoleID, osrole.Name, osrole.Title, osrole.Description, osrole.Powers, count(osgrouprolemembership.AgentID) as Members"
        ." FROM {osrole} AS osrole"
        ." LEFT JOIN {osgrouprolemembership} AS osgrouprolemembership ON (osrole.GroupID = osgrouprolemembership.GroupID AND osrole.RoleID = osgrouprolemembership.RoleID)"
        ." WHERE osrole.GroupID = '%s'"
        ." GROUP BY osrole.RoleID, osrole.Name, osrole.Title, osrole.Description, osrole.Powers";
  $values = array($groupID);

  d4os_io_db_set_active('os_groups');
  $roleResults = db_query($sql, $values);
  if (!$roleResults) {
    d4os_io_db_set_active('default');
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
  d4os_io_db_set_active('default');
  if (!is_array($roles)) {
    return array(
      'error' => t("Could not get group roles"),
      'params' => var_export($params, TRUE)
    );
  }

  if (count($roles) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No roles found for group',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  return $roles;
}

function d4os_io_db_os_groups_services_getGroupRoleMembers($params) {
  global $groupPowers;

  $groupID                    = $params['GroupID'];
  $requestingAgent            = $params['RequestingAgentID'];
  $roleMembersVisibleBit      = $groupPowers['RoleMembersVisible'];
  $canViewAllGroupRoleMembers = d4os_io_services_groups_canAgentViewRoleMembers(
    array(
      'agentID' => $requestingAgent,
      'groupID' => $groupID,
      'roleID'  => ''
    )
  );
  
  $sql = " SELECT osrole.RoleID, osgrouprolemembership.AgentID"
        ." , (osrole.Powers & %d) as MemberVisible"
        ." FROM {osrole} AS osrole"
        ." JOIN {osgrouprolemembership} AS osgrouprolemembership ON (osrole.GroupID = osgrouprolemembership.GroupID AND osrole.RoleID = osgrouprolemembership.RoleID)"
        ." WHERE osrole.GroupID = '%s'";
  $values = array($roleMembersVisibleBit, $groupID);

  d4os_io_db_set_active('os_groups');
  $memberResults = db_query($sql, $values);
  if (!$memberResults) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $members = array();
  $memberResults_count = 0;
  while($member = db_fetch_array($memberResults)) {
    ++$memberResults_count;
    if ($canViewAllGroupRoleMembers || $member['MemberVisible'] || ($member['AgentID'] == $requestingAgent)) {
      $Key = $member['AgentID'] . $member['RoleID'];
      $members[$Key ] = $member;
    }
  }
  d4os_io_db_set_active('default');

  if ($memberResults_count == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No role memberships found for group',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  if (count($members) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No rolememberships visible for group',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  return $members;
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

  $sql = " UPDATE {osgroupmembership} SET"
  ." AgentID = '%s'";
  $values = array($agentID);
  
  if (isset($params['SelectedRoleID'])) {
    $sql .=" , SelectedRoleID = '%s'";
    $values[] = $roleID;
  }
  if (isset($params['AcceptNotices'])) {
    $sql .=" , AcceptNotices = %d";
    $values[] = $acceptNotices;
  }
  if (isset($params['ListInProfile'])) {
    $sql .=" , ListInProfile = %d";
    $values[] = $listInProfile;
  }
  
  $sql .=" WHERE GroupID = '%s' AND AgentID = '%s'";
  $values[] = $groupID;
  $values[] = $agentID;

  d4os_io_db_set_active('os_groups');
  $memberResults = db_query($sql, $values);
  d4os_io_db_set_active('default');
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

  $sql = " SELECT GroupID, NoticeID, Timestamp, FromName, Subject, Message, BinaryBucket"
        ." FROM {osgroupnotice}"
        ." WHERE GroupID = '$groupID'";
  $values = array($groupID);

  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    d4os_io_db_set_active('default');
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
  d4os_io_db_set_active('default');
  if (!is_array($notices)) {
    return array(
      'error' => t("Could not get group notices"),
      'params' => var_export($params, TRUE)
    );
  }
  if (count($notices) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Notices',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  return $notices;
}

function d4os_io_db_os_groups_services_getGroupNotice($params) {
  $noticeID = $params['NoticeID'];

  $sql = " SELECT GroupID, NoticeID, Timestamp, FromName, Subject, Message, BinaryBucket"
        ." FROM {osgroupnotice}"
        ." WHERE NoticeID = '%s'";
  $values = array($noticeID);

  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $notice = db_fetch_array($results);
  d4os_io_db_set_active('default');
  if (!is_array($notice)) {
    return array(
      'error' => t("Could not get group notice"),
      'params' => var_export($params, TRUE)
    );
  }
  if(!is_array($notice)) {
    return array(
      'succeed' => 'false',
      'error' => 'Group Notice Not Found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
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

  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  d4os_io_db_set_active('default');
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
        ." WHERE AgentID = '%s' AND GroupID = '%s'";
  $values = array($agentID, $groupID);

  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    d4os_io_db_set_active('default');
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
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }
  d4os_io_db_set_active('default');
  
  return array('success' => 'true');
}

function d4os_io_db_os_groups_services_getAgentToGroupInvite($params) {
  $inviteID = $params['InviteID'];

  $sql = " SELECT GroupID, RoleID, AgentID FROM {osgroupinvite}"
        ." WHERE InviteID = '%s'";
  $values = array($inviteID);

  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
      'params' => var_export($params, TRUE)
    );
  }

  $inviteInfo = db_fetch_array($results);
  d4os_io_db_set_active('default');

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
        ." WHERE InviteID = '%s'";
  $values = array($inviteID);

  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  d4os_io_db_set_active('default');
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
  d4os_io_db_set_active('os_groups');
  $results = db_query($sql, $values);
  if (!$results) {
    d4os_io_db_set_active('default');
    return array(
      'error' => t("Could not successfully run query (!sql) from DB: ", array('!sql' => $sql)) . db_error(),
    );
  }
  $PermMasked = db_result($results, 0);
  d4os_io_db_set_active('default');
  return $PermMasked;
}