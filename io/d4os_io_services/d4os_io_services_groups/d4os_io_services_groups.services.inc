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

global $groupEnforceGroupPerms, $logLevel;
$groupEnforceGroupPerms = variable_get('d4os_io_services_groups_groupEnforceGroupPerms', 0);
$logLevel = variable_get('d4os_io_services_groups_log_level', 0);

global $groupPowers, $requestingAgent, $common_sig;
$groupPowers = array(
  'None' => '0',
  /// <summary>Can send invitations to groups default role</summary>
  'Invite' => '2',
  /// <summary>Can eject members from group</summary>
  'Eject' => '4',
  /// <summary>Can toggle 'Open Enrollment' and change 'Signup fee'</summary>
  'ChangeOptions' => '8',
  /// <summary>Can create new roles</summary>
  'CreateRole' => '16',
  /// <summary>Can delete existing roles</summary>
  'DeleteRole' => '32',
  /// <summary>Can change Role names, titles and descriptions</summary>
  'RoleProperties' => '64',
  /// <summary>Can assign other members to assigners role</summary>
  'AssignMemberLimited' => '128',
  /// <summary>Can assign other members to any role</summary>
  'AssignMember' => '256',
  /// <summary>Can remove members from roles</summary>
  'RemoveMember' => '512',
  /// <summary>Can assign and remove abilities in roles</summary>
  'ChangeActions' => '1024',
  /// <summary>Can change group Charter, Insignia, 'Publish on the web' and which
  /// members are publicly visible in group member listings</summary>
  'ChangeIdentity' => '2048',
  /// <summary>Can buy land or deed land to group</summary>
  'LandDeed' => '4096',
  /// <summary>Can abandon group owned land to Governor Linden on mainland, or Estate owner for
  /// private estates</summary>
  'LandRelease' => '8192',
  /// <summary>Can set land for-sale information on group owned parcels</summary>
  'LandSetSale' => '16384',
  /// <summary>Can subdivide and join parcels</summary>
  'LandDivideJoin' => '32768',
  /// <summary>Can join group chat sessions</summary>
  'JoinChat' => '65536',
  /// <summary>Can toggle "Show in Find Places" and set search category</summary>
  'FindPlaces' => '131072',
  /// <summary>Can change parcel name, description, and 'Publish on web' settings</summary>
  'LandChangeIdentity' => '262144',
  /// <summary>Can set the landing point and teleport routing on group land</summary>
  'SetLandingPoint' => '524288',
  /// <summary>Can change music and media settings</summary>
  'ChangeMedia' => '1048576',
  /// <summary>Can toggle 'Edit Terrain' option in Land settings</summary>
  'LandEdit' => '2097152',
  /// <summary>Can toggle various About Land > Options settings</summary>
  'LandOptions' => '4194304',
  /// <summary>Can always terraform land, even if parcel settings have it turned off</summary>
  'AllowEditLand' => '8388608',
  /// <summary>Can always fly while over group owned land</summary>
  'AllowFly' => '16777216',
  /// <summary>Can always rez objects on group owned land</summary>
  'AllowRez' => '33554432',
  /// <summary>Can always create landmarks for group owned parcels</summary>
  'AllowLandmark' => '67108864',
  /// <summary>Can use voice chat in Group Chat sessions</summary>
  'AllowVoiceChat' => '134217728',
  /// <summary>Can set home location on any group owned parcel</summary>
  'AllowSetHome' => '268435456',
  /// <summary>Can modify public access settings for group owned parcels</summary>
  'LandManageAllowed' => '536870912',
  /// <summary>Can manager parcel ban lists on group owned land</summary>
  'LandManageBanned' => '1073741824',
  /// <summary>Can manage pass list sales information</summary>
  'LandManagePasses' => '2147483648',
  /// <summary>Can eject and freeze other avatars on group owned land</summary>
  'LandEjectAndFreeze' => '4294967296',
  /// <summary>Can return objects set to group</summary>
  'ReturnGroupSet' => '8589934592',
  /// <summary>Can return non-group owned/set objects</summary>
  'ReturnNonGroup' => '17179869184',
  /// <summary>Can landscape using Linden plants</summary>
  'LandGardening' => '34359738368',
  /// <summary>Can deed objects to group</summary>
  'DeedObject' => '68719476736',
  /// <summary>Can moderate group chat sessions</summary>
  'ModerateChat' => '137438953472',
  /// <summary>Can move group owned objects</summary>
  'ObjectManipulate' => '274877906944',
  /// <summary>Can set group owned objects for-sale</summary>
  'ObjectSetForSale' => '549755813888',
  /// <summary>Pay group liabilities and receive group dividends</summary>
  'Accountable' => '1099511627776',
  /// <summary>Can send group notices</summary>
  'SendNotices'    => '4398046511104',
  /// <summary>Can receive group notices</summary>
  'ReceiveNotices' => '8796093022208',
  /// <summary>Can create group proposals</summary>
  'StartProposal' => '17592186044416',
  /// <summary>Can vote on group proposals</summary>
  'VoteOnProposal' => '35184372088832',
  /// <summary>Can return group owned objects</summary>
  'ReturnGroupOwned' => '281474976710656',
  /// <summary>Members are visible to non-owners</summary>
  'RoleMembersVisible' => '140737488355328'
);

// This is filled in by secure()
$requestingAgent = UUID_ZERO;

// Use a common signature for all the group functions  ->  struct foo($struct)
$common_sig = array(array($xmlrpcStruct, $xmlrpcStruct));

function d4os_io_services_groups_createGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'createGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }
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

  // Create Everyone Role
  // NOTE: FIXME: This is a temp fix until the libomv enum for group powers is fixed in OpenSim
  $result = _d4os_io_services_groups_addRoleToGroup(array('GroupID' => $groupID, 'RoleID' => UUID_ZERO, 'Name' => 'Everyone', 'Description' => 'Everyone in the group is in the everyone role.', 'Title' => "Member of $name", 'Powers' => $everyonePowers));
  if (isset($result['error'])) {
    return $result;
  }

  // Create Owner Role
  $result = _d4os_io_services_groups_addRoleToGroup(array('GroupID' => $groupID, 'RoleID' => $ownerRoleID, 'Name' => 'Owners', 'Description' => "Owners of $name", 'Title' => "Owner of $name", 'Powers' => $ownersPowers));
  if (isset($result['error'])) {
    return $result;
  }

  // Add founder to group, will automatically place them in the Everyone Role, also places them in specified Owner Role
  $result = _d4os_io_services_groups_addAgentToGroup(array('AgentID' => $founderID, 'GroupID' => $groupID, 'RoleID' => $ownerRoleID));
  if (isset($result['error'])) {
    return $result;
  }

  // Select the owner's role for the founder
  $result = _d4os_io_services_groups_setAgentGroupSelectedRole(array('AgentID' => $founderID, 'RoleID' => $ownerRoleID, 'GroupID' => $groupID));
  if (isset($result['error'])) {
    return $result;
  }

  // Set the new group as the founder's active group
  $result = _d4os_io_services_groups_setAgentActiveGroup(array('AgentID' => $founderID, 'GroupID' => $groupID));
  if (isset($result['error'])) {
    return $result;
  }

  $return = d4os_io_services_groups_getGroup(array("GroupID"=>$groupID));
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'createGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld) {
    module_invoke_all('os_group_inworld_create', $return);
  }
  return $return;
}

// Private method, does not include security, to only be called from places that have already verified security
function _d4os_io_services_groups_addRoleToGroup($params, $inworld = TRUE) {
  
  $everyonePowers = 8796495740928; // This should now be fixed, when libomv was updated...		

  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers;
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


function d4os_io_services_groups_addRoleToGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addRoleToGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $groupID = $params['GroupID'];

  // Verify the requesting agent has permission
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['CreateRole']))) {
    return $error;
  }

  $return = _d4os_io_services_groups_addRoleToGroup($params, $inworld);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addRoleToGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_updateGroupRole($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'updateGroupRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $groupID = $params['GroupID'];
  $roleID  = $params['RoleID'];
  $name    = addslashes( $params['Name'] );
  $desc    = addslashes( $params['Description'] );
  $title   = addslashes( $params['Title'] );
  $powers  = $params['Powers'];

  // Verify the requesting agent has permission
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['RoleProperties']))) {
    return $error;
  }

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

  $output = array("success" => "true");
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'updateGroupRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $output;
}

function d4os_io_services_groups_removeRoleFromGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeRoleFromGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $groupID = $params['GroupID'];
  $roleID  = $params['RoleID'];

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['RoleProperties']))) {
    return $error;
  }

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

  $return = array("success" => "true");
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeRoleFromGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroup($params, $inworld = TRUE) {
  global $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  $return = _d4os_io_services_groups_getGroup($params, $inworld);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_getGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent;
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

function d4os_io_services_groups_updateGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'updateGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $name           = $params['Name'];
  $groupID        = $params["GroupID"];
  $charter        = addslashes( $params["Charter"] );
  $insigniaID     = $params["InsigniaID"];
  $membershipFee  = $params["MembershipFee"];
  $openEnrollment = $params["OpenEnrollment"];
  $showInList     = $params["ShowInList"];
  $allowPublish   = $params["AllowPublish"];
  $maturePublish  = $params["MaturePublish"];

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['ChangeOptions']))) {
    return $error;
  }

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
    module_invoke_all('os_group_inworld_update', $params);
  }

  $return = array('success' => 'true');
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'updateGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_findGroups($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'findGroups',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if(is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

  $search = addslashes( $params['Search'] );

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

  $return = array('results' => $results, 'success' => TRUE);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'findGroups',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_setAgentActiveGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent;
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

function d4os_io_services_groups_setAgentActiveGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
       'type' => 'string',
      'data' => 'setAgentActiveGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $agentID = $params['AgentID'];
  $groupID = $params['GroupID'];

  if (isset($requestingAgent) && ($requestingAgent != UUID_ZERO) && ($requestingAgent != $agentID)) {
    return array(
      'error' => "Agent can only change their own Selected Group Role",
      'params' => var_export($params, TRUE)
    );
  }

  $return = _d4os_io_services_groups_setAgentActiveGroup($params, $inworld);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'setAgentActiveGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_addAgentToGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addAgentToGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $groupID = $params["GroupID"];
  $agentID = $params["AgentID"];

  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['AssignMember']))) {
    // If they don't have direct permission, check to see if the group is marked for open enrollment
    $groupInfo = _d4os_io_services_groups_getGroup( array ('GroupID'=>$groupID) );
  
    if( isset($groupInfo['error'])) {
      return $groupInfo;
    }

    if($groupInfo['OpenEnrollment'] != 1) {
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
  
      if(is_array($inviteInfo)) {
        // if there is an invite, make sure we're adding the user to the role specified in the invite
        $params['RoleID'] = $inviteInfo['RoleID'];
      }
      else {
        // Not openenrollment, not invited, return permission denied error
        return $error;
      }
    }
  }

  $return = _d4os_io_services_groups_addAgentToGroup($params, $inworld);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addAgentToGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

// Private method, does not include security, to only be called from places that have already verified security
function _d4os_io_services_groups_addAgentToGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent;
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

  // Make sure they're in the Everyone role
  $result = _d4os_io_services_groups_addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => UUID_ZERO, "AgentID" => $agentID));
  if(isset($result['error'])) {
    return $result;
  }

  // Make sure they're in specified role, if they were invited
  if ($roleID != UUID_ZERO) {
    $result = _d4os_io_services_groups_addAgentToGroupRole(array("GroupID" => $groupID, "RoleID" => $roleID, "AgentID" => $agentID));
    if(isset($result['error'])) {
      return $result;
    }
  }

  //Set the role they were invited to as their selected role
  _d4os_io_services_groups_setAgentGroupSelectedRole(array('AgentID' => $agentID, 'RoleID' => $roleID, 'GroupID' => $groupID));
  
  // Set the group as their active group.
  // _d4os_io_services_groups_setAgentActiveGroup(array("GroupID" => $groupID, "AgentID" => $agentID));
  return array("success" => "true");
}

function d4os_io_services_groups_removeAgentFromGroup($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeAgentFromGroup',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

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

  $return = array("success" => "true");
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeAgentFromGroup',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_addAgentToGroupRole($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent;
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

function d4os_io_services_groups_addAgentToGroupRole($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addAgentToGroupRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

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
  if(!is_array($ownerRoleInfo)) {
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
  
  $return = _d4os_io_services_groups_addAgentToGroupRole($params, $inworld);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addAgentToGroupRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_removeAgentFromGroupRole($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeAgentFromGroupRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID  = $params["RoleID"];
  
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['AssignMember']))) {
    return $error;
  }
  
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

  $return = array("success" => "true");
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeAgentFromGroupRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function _d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent;
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

function d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'setAgentGroupSelectedRole',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $agentID = $params["AgentID"];
  $groupID = $params["GroupID"];
  $roleID = $params["RoleID"];

  if (isset($requestingAgent) && ($requestingAgent != UUID_ZERO) && ($requestingAgent != $agentID)) {
    return array(
      'error' => "Agent can only change their own Selected Group Role",
      'params' => var_export($params, TRUE)
    );
  }

  $return = _d4os_io_services_groups_setAgentGroupSelectedRole($params, $inworld);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'setAgentGroupSelectedRole',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentGroupMembership($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentGroupMembership',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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

  $return = array_merge($groupMembershipInfo, $groupPowersInfo);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentGroupMembership',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentGroupMemberships($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentGroupMemberships',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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

  $return = $groupResults;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentGroupMemberships',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_canAgentViewRoleMembers( $agentID, $groupID, $roleID ) {
  global $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'canAgentViewRoleMembers',
    );
    $log[] = array(
      'name' => 'agentID',
      'type' => 'string',
      'data' => $agentID,
    );
    $log[] = array(
      'name' => 'groupID',
      'type' => 'string',
      'data' => $groupID,
    );
    $log[] = array(
      'name' => 'roleID',
      'type' => 'string',
      'data' => $roleID,
    );
    
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  $membersVisibleTo = variable_get('d4os_io_services_groups_membersVisibleTo', 'Group');
  
  if( $membersVisibleTo == 'All' ) return true;
  
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
    return false;
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
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'canAgentViewRoleMembers',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupMembers($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupMembers',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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

  if ($members == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Group Members found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  if (count($memberResults) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Visible Group Members found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $memberResults;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupMembers',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentActiveMembership($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentActiveMembership',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }
  
  d4os_io_services_groups_secureRequest($params, FALSE);

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

  $return = array_merge($groupMembershipInfo, $groupPowersInfo);
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentActiveMembership',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentRoles($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentRoles',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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
  if (count($roles) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'None found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }

  $return = $roles;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentRoles',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupRoles($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupRoles',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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
  if (count($roles) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No roles found for group',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $roles;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupRoles',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupRoleMembers($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupRoleMembers',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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
  if ($membersCount == 0) {
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

  $return = $members;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupRoleMembers',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_setAgentGroupInfo($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'setAgentGroupInfo',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

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

  $return = array('success'=> 'true');
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'setAgentGroupInfo',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupNotices($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupNotices',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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
  if (count($notices) == 0) {
    return array(
      'succeed' => 'false',
      'error' => 'No Notices',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $notices;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupNotices',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getGroupNotice($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getGroupNotice',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }

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
  
  if(!is_array($notice)) {
    return array(
      'succeed' => 'false',
      'error' => 'Group Notice Not Found',
      'params' => var_export($params, TRUE),
      'sql' => $sql
    );
  }
  
  $return = $notice;
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getGroupNotice',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_addGroupNotice($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addGroupNotice',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $groupID  = $params['GroupID'];
  $noticeID = $params['NoticeID'];
  $fromName = addslashes($params['FromName']);
  $subject  = addslashes($params['Subject']);
  $binaryBucket = $params['BinaryBucket'];
  $message      = addslashes($params['Message']);
  $timeStamp    = $params['TimeStamp'];
  
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['SendNotices']))) {
    return $error;
  }
  
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

  $return = array('success' => 'true');
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addGroupNotice',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_addAgentToGroupInvite($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'addAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

  $inviteID = $params['InviteID'];
  $groupID = $params['GroupID'];
  $roleID  = $params['RoleID'];
  $agentID = $params['AgentID'];
  
  if (is_array($error = d4os_io_services_groups_checkGroupPermission($groupID, $groupPowers['Invite']))) {
    return $error;
  }
  
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
  
  $return = array('success' => 'true');
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'addAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_getAgentToGroupInvite($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'getAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if (is_array($error = d4os_io_services_groups_secureRequest($params, FALSE))) {
    return $error;
  }
  
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
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'getAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_removeAgentToGroupInvite($params, $inworld = TRUE) {
  global $groupEnforceGroupPerms, $requestingAgent, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function call',
      'type' => 'string',
      'data' => 'removeAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'input params',
      'type' => 'array',
      'data' => $params,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  if ($inworld && is_array($error = d4os_io_services_groups_secureRequest($params, TRUE))) {
    return $error;
  }

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

  $return = array('success' => 'true');
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'function output',
      'type' => 'string',
      'data' => 'removeAgentToGroupInvite',
    );
    $log[] = array(
      'name' => 'output values',
      'type' => 'array',
      'data' => $return,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }
  return $return;
}

function d4os_io_services_groups_secureRequest($params, $write = FALSE) {
  global $verifiedReadKey, $verifiedWriteKey, $requestingAgent;
  $overrideAgentUserService = variable_get('d4os_io_services_groups_overrideAgentUserService', '');
  $groupWriteKey = variable_get('d4os_io_services_groups_writeKey', '');
  $groupReadKey = variable_get('d4os_io_services_groups_readKey', '');
  $groupRequireAgentAuthForWrite = variable_get('d4os_io_services_groups_groupRequireAgentAuthForWrite', 0);
  // Cache this for access by other security functions
  $requestingAgent = $params['RequestingAgentID'];
  
  if (isset($groupReadKey) && ($groupReadKey != '') && (!isset($verifiedReadKey) || ($verifiedReadKey !== TRUE))) {
    if (!isset($params['ReadKey']) || ($params['ReadKey'] != $groupReadKey )) {
      return array(
        'error' => "Invalid (or No) Read Key Specified",
        'params' => var_export($params, TRUE)
      );
    }
    else {
      $verifiedReadKey = TRUE;
    }
  }
  
  if (($write == TRUE) && isset($groupWriteKey) && ($groupWriteKey != '') && (!isset($verifiedWriteKey) || ($verifiedWriteKey !== TRUE))) {
    if (!isset($params['WriteKey']) || ($params['WriteKey'] != $groupWriteKey)) {
      return array(
        'error' => "Invalid (or No) Write Key Specified",
        'params' => var_export($params, TRUE)
      );
    }
    else {
      $verifiedWriteKey = TRUE;
    }
  }
  
  if (($write == TRUE) && isset($groupRequireAgentAuthForWrite) && ($groupRequireAgentAuthForWrite == TRUE)) {
    // Note: my brain can't do boolean logic this morning, so just putting this here instead of integrating with line above.
    // If the write key has already been verified for this request, don't check it again.  This comes into play with methods that call other methods, such as CreateGroup() which calls Addrole()
    if (isset($verifiedWriteKey) && ($verifiedWriteKey !== TRUE)) {
      return TRUE;
    }
    
    if( !isset($params['RequestingAgentID']) 
    || !isset($params['RequestingAgentUserService'])
    || !isset($params['RequestingSessionID']) 
    ) {
      return array('error' => "Requesting AgentID and SessionID must be specified", 'params' => var_export($params, TRUE));
    }
    
    // NOTE: an AgentID and SessionID of UUID_ZERO will likely be a region making a request, that is not tied to a specific agent making the request.
    
    $UserService = $params['RequestingAgentUserService'];
    if (isset($overrideAgentUserService) && ($overrideAgentUserService != "")) {
      $UserService = $overrideAgentUserService;
    }
  
    $client = new xmlrpc_client($UserService);
    $client->return_type = 'phpvals';
  
    $verifyParams = new xmlrpcval(array(
      'avatar_uuid' => new xmlrpcval(
        $params['RequestingAgentID'],
        'string'
      ),
      'session_id' => new xmlrpcval(
        $params['RequestingSessionID'],
        'string')
      ),
      'struct'
    );
  
    $message = new xmlrpcmsg("check_auth_session", array($verifyParams));
    $resp = $client->send($message, 5);
    if ($resp->faultCode()) {
      return array(
        'error' => "Error validating AgentID and SessionID",
        'xmlrpcerror'=> $resp->faultString(),
        'params' => var_export($params, TRUE)
      );
    }
  
    $verifyReturn = $resp->value();
  
    if (!isset($verifyReturn['auth_session']) || ($verifyReturn['auth_session'] != 'TRUE')) {
      return array(
        'error' => "UserService.check_auth_session() did not return TRUE",
        'userservice' => var_export($verifyReturn, TRUE),
        'params' => var_export($params, TRUE)
      );
    }
  }
  
  return TRUE;
}

function d4os_io_services_groups_checkGroupPermission($GroupID, $Permission) {
  global $groupEnforceGroupPerms, $requestingAgent, $groupPowers, $logLevel;
  // log
  if ($logLevel > 6) {
    $log = array();
    $log[] = array(
      'name' => 'call function',
      'type' => 'string',
      'data' => 'checkGroupPermission',
    );
    $log[] = array(
      'name' => '$groupEnforceGroupPerms',
      'type' => 'var',
      'data' => $groupEnforceGroupPerms,
    );
    $log[] = array(
      'name' => 'groupID',
      'type' => 'var',
      'data' => $GroupID,
    );
    $log[] = array(
      'name' => 'Permission',
      'type' => 'var',
      'data' => $Permission,
    );
    d4os_log('d4os_group', $log, WATCHDOG_DEBUG);
  }

  if (!isset($Permission) || ($Permission == 0)) {
    return array(
      'error' => 'No Permission value specified for d4os_io_services_groups_checkGroupPermission',
      'Permission' => $Permission
    );
  }
  
  // If it isn't set to true, then always return true, otherwise verify they have perms
  if (!isset($groupEnforceGroupPerms) || ($groupEnforceGroupPerms != TRUE)) {
    return true;
  }
  
  if (!isset($requestingAgent) || ($requestingAgent == UUID_ZERO)) {
    return array(
      'error' => 'Requesting agent was either not specified or not validated.',
      'requestingAgent' => $requestingAgent
    );
  }
  
  $params = array('AgentID' => $requestingAgent, 'GroupID' => $GroupID);
  $reqAgentMembership = d4os_io_services_groups_getAgentGroupMembership($params, $inworld);
  
  if (isset($reqAgentMembership['error'])) {
    return array(
      'error' => 'Could not get agent membership for group',
      'params' => var_export($params, TRUE),
      'nestederror' => $reqAgentMembership['error']
    );
  }
  
  // Worlds ugliest bitwise operation, EVER
  $PermMask   = $reqAgentMembership['GroupPowers'];
  $PermValue  = $Permission;

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
  
  if ($PermMasked != $Permission) {
    $permNames = array_flip($groupPowers);
    return array(
      'error' => 'Agent does not have group power to ' . $Permission .'('.$permNames[$Permission].')',
      'PermMasked' => $PermMasked,
      'params' => var_export($params, TRUE),
      'permBitMaskSql' => $sql,
      'Permission' => $Permission
    );
  }
  
  /*
  return array('error' => 'Reached end'
  , 'reqAgentMembership' => var_export($reqAgentMembership, TRUE)
  , 'GroupID' => $GroupID
  , 'Permission' => $Permission
  , 'PermMasked' => $PermMasked
  );
  */
  return TRUE;
}
