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

// This script was made with the help of RomainFG, Olish Newman, Alex_Kid

define("TYPE_TEXTURES",     0);
define("TYPE_SOUNDS",       1);
define("TYPE_CALLINGCARDS", 2);
define("TYPE_LANDMARKS",    3);
define("TYPE_CLOTHING",     5);
define("TYPE_OBJECTS",      6);
define("TYPE_NOTECARDS",    7);
define("TYPE_SCRIPTS",      10);
define("TYPE_BODYPARTS",    13);
define("TYPE_ANIMATIONS",   20);
define("TYPE_GESTURES",     21);

define("ATTACH_CHEST",      1);  //  chest (roughly the sternum)
define("ATTACH_HEAD",       2);  //  top of head (skull)
define("ATTACH_LSHOULDER",  3);  //  left shoulder
define("ATTACH_RSHOULDER",  4);  //  right shoulder
define("ATTACH_LHAND",      5);  //  left hand
define("ATTACH_RHAND",      6);  //  right hand
define("ATTACH_LFOOT",      7);  //  left foot
define("ATTACH_RFOOT",      8);  //  right foot
define("ATTACH_BACK",       9);  //  spine
define("ATTACH_PELVIS",     10); //  pelvis
define("ATTACH_MOUTH",      11); //  mouth
define("ATTACH_CHIN",       12); //  chin
define("ATTACH_LEAR",       13); //  left ear
define("ATTACH_REAR",       14); //  right ear
define("ATTACH_LEYE",       15); //  left eye
define("ATTACH_REYE",       16); //  right eye
define("ATTACH_NOSE",       17); //  nose
define("ATTACH_RUARM",      18); //  right upper arm
define("ATTACH_RLARM",      19); //  right lower arm
define("ATTACH_LUARM",      20); //  left upper arm
define("ATTACH_LLARM",      21); //  left lower arm
define("ATTACH_RHIP",       22); //  right hip
define("ATTACH_RULEG",      23); //  right upper leg
define("ATTACH_RLLEG",      24); //  right lower leg
define("ATTACH_LHIP",       25); //  left hip
define("ATTACH_LULEG",      26); //  left upper leg
define("ATTACH_LLLEG",      27); //  left lower leg
define("ATTACH_BELLY",      28); //  belly/stomach
define("ATTACH_RPEC",       29); //  right pectoral/breast
define("ATTACH_LPEC",       30); //  left pectoral/breast

define("ATTACH_HUDCENTER2",     31); //  HUD Center 2
define("ATTACH_HUDTOPRIGHT",    32); //  HUD Top Right
define("ATTACH_HUDTOP",         33); //  HUD Top
define("ATTACH_HUDTOPLEFT",     34); //  HUD Top Left
define("ATTACH_HUDCENTER",      35); //  HUD Center
define("ATTACH_HUDBOTTOMLEFT",  36); //  HUD Bottom Left
define("ATTACH_HUDBOTTOM",      37); //  HUD Bottom
define("ATTACH_HUDBOTTOMRIGHT", 38); //  HUD Bottom Right

function d4os_io_db_os_inventory_create_new_user_inventory($params) {
  if (!isset($params['user_uuid'])) return FALSE;

  $user_uuid      = $params['user_uuid'];
  $my_inventory   = isset($params['my_inventory'])    ? $params['my_inventory']   : d4os_uuid_create();
  $animations     = isset($params['animations'])      ? $params['animations']     : d4os_uuid_create();
  $notecards      = isset($params['notecards'])       ? $params['notecards']      : d4os_uuid_create();
  $calling_cards  = isset($params['calling_cards'])   ? $params['calling_cards']  : d4os_uuid_create();
  $landmarks      = isset($params['landmarks'])       ? $params['landmarks']      : d4os_uuid_create();
  $scipts         = isset($params['scripts'])         ? $params['scripts']        : d4os_uuid_create();
  $body_parts     = isset($params['body_parts'])      ? $params['body_parts']     : d4os_uuid_create();
  $sounds         = isset($params['sounds'])          ? $params['sounds']         : d4os_uuid_create();
  $photo_album    = isset($params['photo_album'])     ? $params['photo_album']    : d4os_uuid_create();
  $textures       = isset($params['textures'])        ? $params['textures']       : d4os_uuid_create();
  $gestures       = isset($params['gestures'])        ? $params['gestures']       : d4os_uuid_create();
  $clothing       = isset($params['clothing'])        ? $params['clothing']       : d4os_uuid_create();
  $lost_and_found = isset($params['lost_and_found'])  ? $params['lost_and_found'] : d4os_uuid_create();
  $trash          = isset($params['trash'])           ? $params['trash']          : d4os_uuid_create();
  $objects        = isset($params['objects'])         ? $params['objects']        : d4os_uuid_create();

$values = array(
  $my_inventory,    $user_uuid, UUID_ZERO,
  $animations,      $user_uuid, $my_inventory,
  $notecards,       $user_uuid, $my_inventory,
  $calling_cards,   $user_uuid, $my_inventory,
  $landmarks,       $user_uuid, $my_inventory,
  $scipts,          $user_uuid, $my_inventory,
  $body_parts,      $user_uuid, $my_inventory,
  $sounds,          $user_uuid, $my_inventory,
  $photo_album,     $user_uuid, $my_inventory,
  $textures,        $user_uuid, $my_inventory,
  $gestures,        $user_uuid, $my_inventory,
  $clothing,        $user_uuid, $my_inventory,
  $lost_and_found,  $user_uuid, $my_inventory,
  $trash,           $user_uuid, $my_inventory,
  $objects,         $user_uuid, $my_inventory,
);

$query = "INSERT INTO {inventoryfolders} (`folderName`, `type`, `version`, `folderID`, `agentID`, `parentFolderID`) VALUES"
        . "('My Inventory', 8, 1, '%s', '%s', '00000000-0000-0000-0000-000000000000'),"
        . "('Animations', 20, 1, '%s', '%s', '%s'),"
        . "('Notecards', 7, 1, '%s', '%s', '%s'),"
        . "('Calling Cards', 2, 1, '%s', '%s', '%s'),"
        . "('Landmarks', 3, 1, '%s', '%s', '%s'),"
        . "('Scripts', 10, 1, '%s', '%s', '%s'),"
        . "('Body Parts', 13, 1, '%s', '%s', '%s'),"
        . "('Sounds', 1, 1, '%s', '%s', '%s'),"
        . "('Photo Album', 15, 1, '%s', '%s', '%s'),"
        . "('Textures', 0, 1, '%s', '%s', '%s'),"
        . "('Gestures', 21, 1, '%s', '%s', '%s'),"
        . "('Clothing', 5, 1, '%s', '%s', '%s'),"
        . "('Lost And Found', 16, 1, '%s', '%s', '%s'),"
        . "('Trash', 14, 1, '%s', '%s', '%s'),"
        . "('Objects', 6, 1, '%s', '%s', '%s');";

  d4os_io_db_set_active('os_inventory');
  db_query($query, $values);
  d4os_io_db_set_active('default');
}

function d4os_io_db_os_inventory_create_inventory_item($params) {
  $values = array(
    $params['asset_id'], // 1
    $params['asset_type'], // 2
    $params['inventory_name'], // 3
    $params['inventory_description'], // 4
    $params['inventory_next_permissions'], // 5
    $params['inventory_current_permissions'], // 6
    $params['inventory_type'], // 7
    $params['creator_uuid'], // 8
    $params['inventory_base_permissions'], // 9
    $params['inventory_everyone_permissions'], // 10
    $params['sale_price'], // 11
    $params['sale_type'], // 12
    $params['creation_date'], // 13
    $params['group_id'], // 14
    $params['group_owned'], // 15
    $params['flags'], // 16
    $params['inventory_id'], // 17
    $params['uder_uuid'], // 18
    $params['parent_folder_id'], // 19
  );

  $query = "INSERT INTO {inventoryitems} ("
        . "`assetID`, " // 1
        . "`assetType`, " // 2
        . "`inventoryName`, " // 3
        . "`inventoryDescription`, " // 4
        . "`inventoryNextPermissions`, " // 5
        . "`inventoryCurrentPermissions`, " // 6
        . "`invType`, " // 7
        . "`creatorID`, " // 8
        . "`inventoryBasePermissions`, " // 9
        . "`inventoryEveryOnePermissions`, " // 10
        . "`salePrice`, " // 11
        . "`saleType`, " // 12
        . "`creationDate`, " // 13
        . "`groupID`, " // 14
        . "`groupOwned`, " // 15
        . "`flags`, " // 16
        . "`inventoryID`, " // 17
        . "`avatarID`, " // 18
        . "`parentFolderID`" // 19
        . ") VALUES ("
        . "'%s', " // 1 assetID
        . "%d, " // 2 assetType
        . "'%s', " // 3 inventoryName
        . "'%s', " // 4 inventoryDescription
        . "%d, " // 5 inventoryNextPermissions
        . "%d, " // 6 inventoryCurrentPermissions
        . "%d, " // 7 invType
        . "'%s', " // 8 creatorID
        . "%d, " // 9 inventoryBasePermissions
        . "%d, " // 10 inventoryEveryOnePermissions
        . "%d, " // 11 salePrice
        . "'%s', " // 12 saleType
        . "%d, " // 13 creationDate
        . "'%s', " // 14 groupID
        . "%d, " // 15 groupOwned
        . "%d, " // 16 flags
        . "'%s', " // 17 inventoryID
        . "'%s', " // 18 avatarID
        . "'%s', " // 19 parentFolderID
        . ");";

  d4os_io_db_set_active('os_inventory');
  db_query($query, $values);
  d4os_io_db_set_active('default');
}

function d4os_io_db_os_inventory_create_user_attachment($params) {
  $values = array(
    $params['user_uuid'],
    $params['attachment_point'],
    $params['inventory_id'],
    $params['random_asset_uuid'],
  );
  $query = "INSERT INTO {avatarattachments} (`UUID`, `attachpoint`, `item`, `asset`) VALUES ('%s', %d, '%s', '%s');";

  d4os_io_db_set_active('os_users');
  db_query($query, $values);
  d4os_io_db_set_active('default');
}

function d4os_io_db_os_inventory_create_user_wearable($params) {
  $values = array(
    $params['Owner'], // 1
    $params['Serial'], // 2
    $params['Visual_Params'], // 3
    $params['Texture'], // 4
    $params['Avatar_Height'], // 5
    $params['Body_Item'], // 6
    $params['Body_Asset'], // 7
    $params['Skin_Item'], // 8
    $params['Skin_Asset'], // 9
    $params['Hair_Item'], // 10
    $params['Hair_Asset'], // 11
    $params['Eyes_Item'], // 12
    $params['Eyes_Asset'], // 13
    $params['Shirt_Item'], // 14
    $params['Shirt_Asset'], // 15
    $params['Pants_Item'], // 16
    $params['Pants_Asset'], // 17
    $params['Shoes_Item'], // 18
    $params['Shoes_Asset'], // 19
    $params['Socks_Item'], // 20
    $params['Socks_Asset'], // 21
    $params['Jacket_Item'], // 22
    $params['Jacket_Asset'], // 23
    $params['Gloves_Item'], // 24
    $params['Gloves_Asset'], // 25
    $params['Undershirt_Item'], // 26
    $params['Undershirt_Asset'], // 27
    $params['Underpants_Item'], // 28
    $params['Underpants_Asset'], // 29
    $params['Skirt_Item'], // 30
    $params['Skirt_Asset'] // 31
  );
  $query = "INSERT INTO {avatarappearance} ("
  . "`Owner`, " // 1
  . "`Serial`, " // 2
  . "`Visual_Params`, " // 3
  . "`Texture`, " // 4
  . "`Avatar_Height`," // 5
  . "`Body_Item`, " // 6
  . "`Body_Asset`, " // 7
  . "`Skin_Item`, " // 8
  . "`Skin_Asset`, " // 9
  . "`Hair_Item`, " // 10
  . "`Hair_Asset`, " // 11
  . "`Eyes_Item`, " // 12
  . "`Eyes_Asset`, " // 13
  . "`Shirt_Item`, " // 14
  . "`Shirt_Asset`, " // 15
  . "`Pants_Item`, " // 16
  . "`Pants_Asset`, " // 17
  . "`Shoes_Item`, " // 18
  . "`Shoes_Asset`, " // 19
  . "`Socks_Item`, " // 20
  . "`Socks_Asset`, " // 21
  . "`Jacket_Item`, " // 22
  . "`Jacket_Asset`, " // 23
  . "`Gloves_Item`, " // 24
  . "`Gloves_Asset`, " // 25
  . "`Undershirt_Item`, " // 26
  . "`Undershirt_Asset`, " // 27
  . "`Underpants_Item`, " // 28
  . "`Underpants_Asset`, " // 29
  . "`Skirt_Item`, " // 30
  . "`Skirt_Asset`" // 31
  . ") VALUES ("
  . "%d, " // 1 Owner // int
  . "%b, " // 2 Serial // blob
  . "%b, " // 3 Visual_Params // blob
  . "%f, " // 4 Texture // float
  . "'%s', " // 5 Avatar_Height
  . "'%s', " // 6 Body_Item
  . "'%s', " // 7 Body_Asset
  . "'%s', " // 8 Skin_Item
  . "'%s', " // 9 Skin_Asset
  . "'%s', " // 10 Hair_Item
  . "'%s', " // 11 Hair_Asset
  . "'%s', " // 12 Eyes_Item
  . "'%s', " // 13 Eyes_Asset
  . "'%s', " // 14 Shirt_Item
  . "'%s', " // 15 Shirt_Asset
  . "'%s', " // 16 Pants_Item
  . "'%s', " // 17 Pants_Asset
  . "'%s', " // 18 Shoes_Item
  . "'%s', " // 19 Shoes_Asset
  . "'%s', " // 20 Socks_Item
  . "'%s', " // 21 Socks_Asset
  . "'%s', " // 22 Jacket_Item
  . "'%s', " // 23 Jacket_Asset
  . "'%s', " // 24 Gloves_Item
  . "'%s', " // 25 Gloves_Asset
  . "'%s', " // 26 Undershirt_Item
  . "'%s', " // 27 Undershirt_Asset
  . "'%s', " // 28 Underpants_Item
  . "'%s', " // 29 Underpants_Asset
  . "'%s', " // 30 Skirt_Item
  . "'%s', " // 31 Skirt_Asset
  . "');";

  d4os_io_db_set_active('os_users');
  db_query($query, $values);
  d4os_io_db_set_active('default');
}

function d4os_io_db_os_inventory_get_inventory_folder_by_path($params) {
  // build the query
  $values = array($params['user_uuid']);
  $query = "SELECT * FROM {inventoryfolders} WHERE agentID = '%s'";

  // request the data
  d4os_io_db_set_active('os_inventory');
  $result = db_query($query, $values);
  $user_folders = array();
  while ($folder = db_fetch_array($result)) {
    $user_folders[] = $folder;
  }
  d4os_io_db_set_active('default');

  // no folder for the user, return an error
  if (count($user_folders) == 0) {
    return array(
      'success' => FALSE,
      'message' => t('Could not find folders for this user.')
    );
  }

  // get the parents
  $params['user_folders'] = $user_folders;
  $parents_folders = d4os_io_db_inventory_folder_get_parents_by_path($params);
  $folder_uuid = array_pop($parents_folders);

  $data = array(
    'user_folders'=> $user_folders,
    'folder_uuid' => $folder_uuid,
    'parents'     => $parents_folders,
  );

  return array(
    'success' => TRUE,
    'message' => '',
    'data'    => $data
  );
}

function d4os_io_db_inventory_folder_get_parents_by_path($params) {
  // get path args
  $args = explode('/', $params['path']);
  // get the first folder
  $parents_folders = array();
  foreach($params['user_folders'] as $k => $v) {
    if ($v['folderName'] == 'My Inventory' && $v['parentFolderID'] = UUID_ZERO) {
      $parents_folders[] = $v['folderID'];
      break;
    }
  }
  // get the parents
  foreach ($args as $arg) {
    foreach ($params['user_folders'] as $k => $v) {
      if ($v['folderName'] == $arg && $v['parentFolderID'] = end($parents_folders)) {
        $parents_folders[] = $v['folderID'];
        break;
      }
    }
  }
  return $parents_folders;
}

function d4os_io_db_inventory_clone_folders($params) {
  $new_folders = array();
  // get the uuids
  $old_folders_uuids = array();
  foreach($params['user_folders'] as $k => $folder) {
    $old_folders_uuids[] = $folder['folderID'];
    if ($folder['parentFolderID'] != UUID_ZERO) {
      $old_folders_uuids[] = $folder['parentFolderID'];
    }
  }
  $old_folders_uuids = array_unique($old_folders_uuids);
  sort($old_folders_uuids);

  // generate new uuids
  $new_folders_uuids = array();
  foreach($old_folders_uuids as $uuid) {
    $new_folders_uuids[] = d4os_uuid_create();
  }

  // replace the uuids
  foreach($params['user_folders'] as $folder) {
    $values = $folder;

    $new_uuid_id        = array_search($folder['folderID'], $old_folders_uuids);
    $values['folderID'] = $new_folders_uuids[$new_uuid_id];

    if ($folder['parentFolderID'] != UUID_ZERO) {
      $new_parent_uuid_id = array_search($folder['parentFolderID'], $old_folders_uuids);
      $values['parentFolderID'] = $new_folders_uuids[$new_parent_uuid_id];
    }

    $values['agentID'] = $params['avatar_dest_uuid'];
    $new_folders[] = $values;
  }

  $data = $params;
  $data['old_folders_uuids']  = $old_folders_uuids;
  $data['new_folders_uuids']  = $new_folders_uuids;
  $data['new_folders']        = $new_folders;

  return array(
    'success' => TRUE,
    'message' => '',
    'data'    => $data
  );
}

function d4os_io_db_inventory_clone_items($params) {
  $new_items = array();

  // get the uuids
  $old_items_uuids = array();
  foreach($params['user_items'] as $k => $item) {
    $old_items_uuids[] = $item['inventoryID'];
  }
  $old_items_uuids = array_unique($old_items_uuids);
  sort($old_items_uuids);

  // generate new uuids
  $new_items_uuids = array();
  foreach($old_items_uuids as $uuid) {
    $new_items_uuids[] = d4os_uuid_create();
  }

  // replace the uuids
  foreach($params['user_items'] as $k => $item) {
    $new_uuid_id        = array_search($item['inventoryID'], $old_items_uuids);
    $new_parent_uuid_id = array_search($item['parentFolderID'], $params['old_folders_uuids']);
    $values = $item;
    $values['inventoryID']    = $new_items_uuids[$new_uuid_id];
    $values['avatarID']       = $params['avatar_dest_uuid'];
    $values['parentFolderID'] = $params['new_folders_uuids'][$new_parent_uuid_id];
    $new_items[] = $values;
  }

  $data = $params;
  $data['old_items_uuids'] = $old_items_uuids;
  $data['new_items_uuids'] = $new_items_uuids;
  $data['new_items'] = $new_items;

  return array(
    'success' => TRUE,
    'message' => '',
    'data'    => $data
  );
}

function d4os_io_db_os_inventory_clone_inventory($params) {
  // delete actual folders and items
  d4os_io_db_set_active('os_inventory');
  db_query("DELETE FROM {inventoryfolders} WHERE agentID = '%s'", array($params['avatar_dest_uuid']));
  db_query("DELETE FROM {inventoryitems} WHERE avatarID = '%s'", array($params['avatar_dest_uuid']));
  d4os_io_db_set_active('default');

  // get the folders
  $values = array($params['avatar_src_uuid']);
  $query = "SELECT * FROM {inventoryfolders} WHERE agentID = '%s'";

  // request the data
  d4os_io_db_set_active('os_inventory');
  $result = db_query($query, $values);
  $user_folders = array();
  while ($folder = db_fetch_array($result)) {
    $user_folders[] = $folder;
  }
  d4os_io_db_set_active('default');

  // no folder for the user, return an error
  if (count($user_folders) == 0) {
    return array(
      'success' => FALSE,
      'message' => t('Could not find folders for this user.')
    );
  }

  // clone the folders
  $params['user_folders'] = $user_folders;
  $new_folders = d4os_io_db_inventory_clone_folders($params);

  // get the items
  $query = "SELECT * FROM {inventoryitems} WHERE avatarID = '%s'";

  // request the data
  d4os_io_db_set_active('os_inventory');
  $result = db_query($query, $values);
  $user_items = array();
  while ($item = db_fetch_array($result)) {
    $user_items[] = $item;
  }
  d4os_io_db_set_active('default');

  // no items for the user, return an error
  if (count($user_items) == 0) {
    return array(
      'success' => FALSE,
      'message' => t('Could not find items for this user.')
    );
  }

  // clone the items
  $params['user_items']   = $user_items;
  $params = array_merge($params, $new_folders['data']);
  $new_items = d4os_io_db_inventory_clone_items($params);
  $params = array_merge($params, $new_items['data']);

  // build the query for folders
  $folders_list_values = array();
  $folders_list_vars = '';
  foreach ($new_items['data']['new_folders'] as $new_folder) {
    $folders_list_vars .= "('%s', %d, %d, '%s', '%s', '%s'),";
    $folders_list_values[] = $new_folder['folderName'];
    $folders_list_values[] = $new_folder['type'];
    $folders_list_values[] = $new_folder['version'];
    $folders_list_values[] = $new_folder['folderID'];
    $folders_list_values[] = $new_folder['agentID'];
    $folders_list_values[] = $new_folder['parentFolderID'];
  }
  // remove the trailing ,
  $folders_list_vars = substr($folders_list_vars, 0, -1);

  // fill the base with all this data
  $query = "INSERT INTO {inventoryfolders} ("
          . "folderName,"
          . "type,"
          . "version,"
          . "folderID,"
          . "agentID,"
          . "parentFolderID"
          . ") VALUES ". $folders_list_vars;
  d4os_io_db_set_active('os_inventory');
  db_query($query, $folders_list_values);
  d4os_io_db_set_active('default');

  // build the query for items
  $items_list_values = array();
  $items_list_vars = '';
  foreach ($new_items['data']['new_items'] as $new_item) {
    $items_list_vars .= "("
                      . "'%s'," // 1
                      . "%d," // 2
                      . "'%s'," // 3
                      . "'%s'," // 4
                      . "%d," // 5
                      . "%d," // 6
                      . "%d," // 7
                      . "'%s'," // 8
                      . "%d," // 9
                      . "%d," // 10
                      . "%d," // 11
                      . "%d," // 12
                      . "%d," // 13
                      . "'%s'," // 14
                      . "%d," // 15
                      . "%d," // 16
                      . "'%s'," // 17
                      . "'%s'," // 18
                      . "'%s'," // 19
                      . "%d" // 20
                      . "),";
    $items_list_values[] = $new_item['assetID']; // 1
    $items_list_values[] = $new_item['assetType']; // 2 int
    $items_list_values[] = $new_item['inventoryName']; // 3
    $items_list_values[] = $new_item['inventoryDescription']; // 4
    $items_list_values[] = $new_item['inventoryNextPermissions']; // 5 int
    $items_list_values[] = $new_item['inventoryCurrentPermissions']; // 6 int
    $items_list_values[] = $new_item['invType']; // 7 int
    $items_list_values[] = $new_item['creatorID']; // 8
    $items_list_values[] = $new_item['inventoryBasePermissions']; // 9 int
    $items_list_values[] = $new_item['inventoryEveryOnePermissions']; // 10 int
    $items_list_values[] = $new_item['salePrice']; // 11 int
    $items_list_values[] = $new_item['saleType']; // 12 int
    $items_list_values[] = $new_item['creationDate']; // 13 int
    $items_list_values[] = $new_item['groupID']; // 14
    $items_list_values[] = $new_item['groupOwned']; // 15 int
    $items_list_values[] = $new_item['flags']; // 16 int
    $items_list_values[] = $new_item['inventoryID']; // 17
    $items_list_values[] = $new_item['avatarID']; // 18
    $items_list_values[] = $new_item['parentFolderID']; // 19
    $items_list_values[] = $new_item['inventoryGroupPermissions']; // 20 int
  }
  // remove the trailing ,
  $items_list_vars = substr($items_list_vars, 0, -1);

  // fill the base with all this data
  $query = "INSERT INTO {inventoryitems} ("
          . "assetID," // 1
          . "assetType," // 2
          . "inventoryName," // 3
          . "inventoryDescription," // 4
          . "inventoryNextPermissions," // 5
          . "inventoryCurrentPermissions," // 6
          . "invType," // 7
          . "creatorID," // 8
          . "inventoryBasePermissions," // 9
          . "inventoryEveryOnePermissions," // 10
          . "salePrice," // 11
          . "saleType," // 12
          . "creationDate," // 13
          . "groupID," // 14
          . "groupOwned," // 15
          . "flags," // 16
          . "inventoryID," // 17
          . "avatarID," // 18
          . "parentFolderID," // 19
          . "inventoryGroupPermissions" // 20
          . ") VALUES ". $items_list_vars;
  d4os_io_db_set_active('os_inventory');
  db_query($query, $items_list_values);
  d4os_io_db_set_active('default');

  return array(
    'success' => TRUE,
    'message' => '',
    'data' => $params
  );
}

function d4os_io_db_os_inventory_clone_appearance($params) {
  // delete actual appearance
  d4os_io_db_set_active('os_users');
  db_query("DELETE FROM {avatarappearance} WHERE Owner = '%s'", array($params['avatar_dest_uuid']));
  d4os_io_db_set_active('default');

  // get the folders
  $values = array($params['avatar_src_uuid']);
  $query = "SELECT * FROM {avatarappearance} WHERE Owner = '%s'";

  // request the data
  d4os_io_db_set_active('os_users');
  $appearance = db_fetch_array(db_query($query, $values));
  d4os_io_db_set_active('default');

  // get the new uuids for items
  $old_items_uuids = $params['old_items_uuids'];
  $new_items_uuids = $params['new_items_uuids'];
  $new_uuid_id                = array_search($item['Body_Item'],        $old_items_uuids);
  $appearance['Body_Item']        = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Skin_Item'],        $old_items_uuids);
  $appearance['Skin_Item']        = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Hair_Item'],        $old_items_uuids);
  $appearance['Hair_Item']        = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Eyes_Item'],        $old_items_uuids);
  $appearance['Eyes_Item']        = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Shirt_Item'],       $old_items_uuids);
  $appearance['Shirt_Item']       = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Pants_Item'],       $old_items_uuids);
  $appearance['Pants_Item']       = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Shoes_Item'],       $old_items_uuids);
  $appearance['Shoes_Item']       = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Socks_Item'],       $old_items_uuids);
  $appearance['Socks_Item']       = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Jacket_Item'],      $old_items_uuids);
  $appearance['Jacket_Item']      = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Gloves_Item'],      $old_items_uuids);
  $appearance['Gloves_Item']      = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Undershirt_Item'],  $old_items_uuids);
  $appearance['Undershirt_Item']  = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Underpants_Item'],  $old_items_uuids);
  $appearance['Underpants_Item']  = $new_items_uuids[$new_uuid_id];
  $new_uuid_id                = array_search($item['Skirt_Item'],       $old_items_uuids);
  $appearance['Skirt_Item']       = $new_items_uuids[$new_uuid_id];
  
  // fill the base with all this data
  $values = array(
    $params['avatar_dest_uuid'], // 1
    $appearance['Serial'], // 2
    $appearance['Visual_Params'], // 3
    $appearance['Texture'], // 4
    $appearance['Avatar_Height'], // 5
    $appearance['Body_Item'], // 6
    $appearance['Body_Asset'], // 7
    $appearance['Skin_Item'], // 8
    $appearance['Skin_Asset'], // 9
    $appearance['Hair_Item'], // 10
    $appearance['Hair_Asset'], // 11
    $appearance['Eyes_Item'], // 12
    $appearance['Eyes_Asset'], // 13
    $appearance['Shirt_Item'], // 14
    $appearance['Shirt_Asset'], // 15
    $appearance['Pants_Item'], // 16
    $appearance['Pants_Asset'], // 17
    $appearance['Shoes_Item'], // 18
    $appearance['Shoes_Asset'], // 19
    $appearance['Socks_Item'], // 20
    $appearance['Socks_Asset'], // 21
    $appearance['Jacket_Item'], // 22
    $appearance['Jacket_Asset'], // 23
    $appearance['Gloves_Item'], // 24
    $appearance['Gloves_Asset'], // 25
    $appearance['Undershirt_Item'], // 26
    $appearance['Undershirt_Asset'], // 27
    $appearance['Underpants_Item'], // 28
    $appearance['Underpants_Asset'], // 29
    $appearance['Skirt_Item'], // 30
    $appearance['Skirt_Asset'], // 31
  );
  $query = "INSERT INTO {avatarappearance} ("
          . "Owner," // 1
          . "Serial," // 2 int
          . "Visual_Params," // 3 blob
          . "Texture," // 4 blob
          . "Avatar_Height," // 5 float
          . "Body_Item," // 6
          . "Body_Asset," // 7
          . "Skin_Item," // 8
          . "Skin_Asset," // 9
          . "Hair_Item," // 10
          . "Hair_Asset," // 11
          . "Eyes_Item," // 12
          . "Eyes_Asset," // 13
          . "Shirt_Item," // 14
          . "Shirt_Asset," // 15
          . "Pants_Item," // 16
          . "Pants_Asset," // 17
          . "Shoes_Item," // 18
          . "Shoes_Asset," // 19
          . "Socks_Item," // 20
          . "Socks_Asset," // 21
          . "Jacket_Item," // 22
          . "Jacket_Asset," // 23
          . "Gloves_Item," // 24
          . "Gloves_Asset," // 25
          . "Undershirt_Item," // 26
          . "Undershirt_Asset," // 27
          . "Underpants_Item," // 28
          . "Underpants_Asset," // 29
          . "Skirt_Item," // 30
          . "Skirt_Asset" // 31
          . ") VALUES ("
          . "'%s'," // 1
          . "%d," // 2
          . "%b," // 3
          . "%b," // 4
          . "%f," // 5
          . "'%s'," // 6
          . "'%s'," // 7
          . "'%s'," // 8
          . "'%s'," // 9
          . "'%s'," // 10
          . "'%s'," // 11
          . "'%s'," // 12
          . "'%s'," // 13
          . "'%s'," // 14
          . "'%s'," // 15
          . "'%s'," // 16
          . "'%s'," // 17
          . "'%s'," // 18
          . "'%s'," // 19
          . "'%s'," // 20
          . "'%s'," // 21
          . "'%s'," // 22
          . "'%s'," // 23
          . "'%s'," // 24
          . "'%s'," // 25
          . "'%s'," // 26
          . "'%s'," // 27
          . "'%s'," // 28
          . "'%s'," // 29
          . "'%s'," // 30
          . "'%s'" // 31
          . ")";
  d4os_io_db_set_active('os_users');
  db_query($query, $values);
  d4os_io_db_set_active('default');

  return array(
    'success' => TRUE,
    'message' => '',
    'data' => $params
  );
}

function d4os_io_db_os_inventory_clone_attachments($params) {
  // delete actual attachments
  d4os_io_db_set_active('os_users');
  db_query("DELETE FROM {avatarattachments} WHERE UUID = '%s'", array($params['avatar_dest_uuid']));
  d4os_io_db_set_active('default');

  // get the folders
  $values = array($params['avatar_src_uuid']);
  $query = "SELECT * FROM {avatarattachments} WHERE UUID = '%s'";

  $attachments = array();

  // request the data
  d4os_io_db_set_active('os_users');
  $result = db_query($query, $values);
  while ($attachment = db_fetch_array($result)) {
    $attachments[] = $attachment;
  }
  d4os_io_db_set_active('default');

  // change the items uuids
  $new_attachments = array();
  foreach($attachments as $attachment) {
    $new_attachment = $attachment;
    $new_uuid_id = array_search($attachment['item'], $params['old_items_uuids']);
    $new_attachment['item'] = $params['new_items_uuids'][$new_uuid_id];
    $new_attachments[] = $new_attachment;
  }

  // fill the base with all this data
  $attachments_list_vars = '';
  $values = array();
  foreach($new_attachments as $attachment) {
    $attachments_list_vars .= "('%s', %d, '%s', '%s'),";
    $values[] = $params['avatar_dest_uuid'];
    $values[] = $attachment['attachpoint'];
    $values[] = $attachment['item'];
    $values[] = $attachment['asset'];
  }
  // remove the trailing ,
  $attachments_list_vars = substr($attachments_list_vars, 0, -1);

  if (count($attachments_list_vars) == 0) {
    return array(
      'success' => FALSE,
      'message' => t('No attachments found.'),
      'data' => $params
    );
  }
  $query = "INSERT INTO {avatarattachments} ("
          . "UUID," // 1
          . "attachpoint," // 2 int
          . "item," // 3
          . "asset" // 4
          . ") VALUES ". $attachments_list_vars;

  d4os_io_db_set_active('os_users');
  db_query($query, $values);
  d4os_io_db_set_active('default');

  drupal_set_message(t('Appearance attached.'));
  return array(
    'success' => TRUE,
    'message' => '',
    'data' => $params
  );
}

function d4os_io_db_os_inventory_clone_model($params) {
  $params = d4os_io_db_os_inventory_clone_inventory($params);
drupal_set_message(t('Inventory folders created and default items added.'));
  $params = d4os_io_db_os_inventory_clone_appearance($params['data']);
drupal_set_message(t('Appearance added.'));
  $params = d4os_io_db_os_inventory_clone_attachments($params['data']);
}

function d4os_io_db_os_inventory_user_delete($uuid) {
  $values = array($uuid);
  // delete attachments and appearance
  d4os_io_db_set_active('os_users');
  db_query("DELETE FROM {avatarattachments} WHERE UUID = '%s'", $values);
  db_query("DELETE FROM {avatarappearance} WHERE Owner = '%s'", $values);
  d4os_io_db_set_active('default');
  // delete folders and items
  d4os_io_db_set_active('os_inventory');
  db_query("DELETE FROM {inventoryfolders} WHERE agentID = '%s'", $values);
  db_query("DELETE FROM {inventoryitems} WHERE avatarID = '%s'", $values);
  d4os_io_db_set_active('default');
}

function d4os_io_db_os_inventory_is_alive() {
  return array(
    'success' => d4os_io_db_mysql_is_alive('os_inventory', 'os inventory'),
  );
}