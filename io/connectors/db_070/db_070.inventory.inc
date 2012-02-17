<?php
/**
 * @package    d4os_io db 070
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

class D4OS_IO_db_070_Inventory implements D4OS_IO_Inventory_Interface {
	public $response;
	public $values;

	function __construct() {
		$this->values = array(

		);
	}

	function ping() {
		$this->response->success = d4os_io_db_070_mysql_is_alive('os_robust', 'Robust');
		return;
	}

	function create_new_inventory($params) {
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
          . "('My Inventory', 8, 1, '%s', '%s', '%s'),"
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

    d4os_io_db_070_set_active('os_robust');
    db_query($query, $values);
    d4os_io_db_070_set_active('default');
  }

  function create_inventory_item($params) {
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

    d4os_io_db_070_set_active('os_robust');
    db_query($query, $values);
    d4os_io_db_070_set_active('default');
  }

  function create_user_attachment($params) {
    $values = array(
      $params['user_uuid'],
      $params['attachment_point'],
      $params['inventory_id'],
      $params['random_asset_uuid'],
    );
    $query = "INSERT INTO {avatarattachments} (`UUID`, `attachpoint`, `item`, `asset`) VALUES ('%s', %d, '%s', '%s');";

    d4os_io_db_070_set_active('os_users');
    db_query($query, $values);
    d4os_io_db_070_set_active('default');
  }

  function create_user_wearable($params) {
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

    d4os_io_db_070_set_active('os_users');
    db_query($query, $values);
    d4os_io_db_070_set_active('default');
  }

  function get_inventory_folder_by_path($params) {
    // build the query
    $values = array($params['user_uuid']);
    $query = "SELECT * FROM {inventoryfolders} WHERE agentID = '%s'";

    // request the data
    d4os_io_db_070_set_active('os_robust');
    $result = db_query($query, $values);
    $user_folders = array();
    while ($folder = db_fetch_array($result)) {
      $user_folders[] = $folder;
    }
    d4os_io_db_070_set_active('default');

    // no folder for the user, return an error
    if (count($user_folders) == 0) {
      return array(
        'success' => FALSE,
        'message' => t('Could not find folders for this user.')
      );
    }

    // get the parents
    $params['user_folders'] = $user_folders;
    $parents_folders = $this->get_folder_parents_by_path($params);
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

  function get_folder_parents_by_path($params) {
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

  function clone_folders($params) {
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

  function clone_items($params) {
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

  /**
  * @todo : pass by reference
  */
  function clone_inventory($params) {
    // delete actual folders and items
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {inventoryfolders} WHERE agentID = '%s'", array($params['avatar_dest_uuid']));
    db_query("DELETE FROM {inventoryitems} WHERE avatarID = '%s'", array($params['avatar_dest_uuid']));
    d4os_io_db_070_set_active('default');

    // get the folders
    $user_folders = array();
    d4os_io_db_070_set_active('os_robust');
    $result = db_query("SELECT * FROM {inventoryfolders} WHERE agentID = '%s'", $params['avatar_src_uuid']);
    while ($folder = db_fetch_array($result)) {
      $user_folders[] = $folder;
    }
    d4os_io_db_070_set_active('default');

    // no folder for the user, return an error
    if (count($user_folders) == 0) {
      return array(
        'success' => FALSE,
        'message' => t('Could not find folders for this user.')
      );
    }

    // clone the folders
    $params['user_folders'] = $user_folders;
    $new_folders = $this->clone_folders($params);

    // get the items
    $user_items = array();
    d4os_io_db_070_set_active('os_robust');
    $result = db_query("SELECT * FROM {inventoryitems} WHERE avatarID = '%s'", $params['avatar_src_uuid']);
    while ($item = db_fetch_array($result)) {
      $user_items[] = $item;
    }
    d4os_io_db_070_set_active('default');

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
    $new_items = $this->clone_items($params);
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
    d4os_io_db_070_set_active('os_robust');
    db_query($query, $folders_list_values);
    d4os_io_db_070_set_active('default');

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
    d4os_io_db_070_set_active('os_robust');
    db_query($query, $items_list_values);
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'message' => '',
      'data' => $params
    );
  }

  /**
  * @todo : pass by reference
  */
  function clone_appearance($params) {
    // delete actual appearance
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {Avatars} WHERE PrincipalID = '%s'", array($params['avatar_dest_uuid']));
    d4os_io_db_070_set_active('default');

    // get the folders
    $appearance = array();
    d4os_io_db_070_set_active('os_robust');
    $result = db_query("SELECT Name, Value FROM {Avatars} WHERE PrincipalID = '%s'", $params['avatar_src_uuid']);
    while ($item = db_fetch_array($result)) {
      $appearance[] = $item;
    }
    d4os_io_db_070_set_active('default');

    // no items for the user, return an error
    if (count($appearance) == 0) {
      return array(
        'success' => FALSE,
        'message' => t('Could not find appearance for this user.')
      );
    }

    // take values
  /*
  INSERT INTO `Avatars` (`PrincipalID`, `Name`, `Value`) VALUES
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'AvatarHeight', '1.990715'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'AvatarType', '1'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'METHOD', 'setavatar'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Serial', '0'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'UserID', 'f775ad73-cb26-4d76-ac1c-58d8593b54d1'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'VERSIONMAX', '0'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'VERSIONMIN', '0'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'VisualParams', '17,15,85,0,58,188,35,150,25,0,0,71,63,36,85,58,153,51,134,0,73,61,88,132,63,255,55,0,86,136,33,255,255,255,0,127,0,0,127,0,0,127,0,0,0,127,114,127,99,63,127,140,127,127,0,0,0,191,0,104,0,0,0,0,0,0,0,0,0,145,216,133,0,53,0,155,130,0,0,127,127,109,0,0,0,63,56,0,150,150,150,150,150,150,150,61,150,150,150,45,119,0,0,153,152,127,181,127,122,0,127,127,127,127,127,132,59,68,22,96,124,118,47,127,137,127,63,63,0,0,0,0,127,127,0,0,0,0,127,0,159,0,0,178,127,36,85,131,56,127,127,153,165,0,0,74,0,76,127,0,150,150,255,0,0,107,30,127,255,255,255,255,255,255,255,255,255,0,0,255,255,79,0,150,150,150,150,150,150,150,150,150,0,0,0,0,150,150,150,42,127,127,213,150,150,150,150,150,150,150,0,0,150,51,50,150,150,150'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 0:0', '49901b4c-64d5-4aee-b21c-5675026cf2b0:ea295fb0-ee17-405a-96ee-11e76b4c6568'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 1:0', '9bc3742a-662b-46e9-a407-e29344a62eee:0f3d20c7-9487-4604-b4b6-89808539de88'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 2:0', 'e1d5c309-a2f8-4f64-86e4-8acfbcd9bc3c:e75fab8a-16aa-4c42-8fa5-6e961053a5c0'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 3:0', 'a1aefbac-5f9a-4f94-b51f-0368defbce8e:3bdf0ce3-c7ba-4569-b951-bf0db63c94d0'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 4:0', 'd2bd636d-70ec-40da-aa54-b5589845d55f:d188e0eb-43b2-4df0-803b-221b485eaecb'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 5:0', 'a2669522-c2fa-4344-a1d7-4b0444b2f16c:d6439e46-44ae-420b-a34b-bb0061c0b949'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 6:0', '97ba86ad-94c5-4915-9257-255e09b6b140:094c171c-f423-40a6-9c6b-8be7b9d18514'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 9:0', '2ff12cd5-ec1d-4096-8f80-170f6c91bb26:6ebee605-b2c9-4475-970f-11c08e48c979'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', '_ap_2', '0645e5ad-6ae4-4e3f-a976-7c2e0e76660c'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', '_ap_7', 'e8394d4f-a1c3-481c-b2e2-6609307c17c0'),
  ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', '_ap_8', '73915a4e-9743-4897-aa66-f10fee793b25');
  UUID_INVENTORY_ITEM:UUID_ASSET
  */
    // fill the base with all this data
    $queries  = array();
    $values   = array();

    for ($i=0; $i < count($appearance); $i++) {
      if (strpos($appearance[$i]['Name'], 'Wearable') !== FALSE) {
        $uuids = explode(":", $appearance[$i]['Value']);
        $item = $uuids[0];
        $asset = $uuids[1];
        $query = "SELECT inventoryID FROM {inventoryitems} WHERE assetID='%s' AND avatarID='%s'";

        d4os_io_db_070_set_active('os_robust');
        $result = db_fetch_object(db_query($query, $asset, $params['avatar_dest_uuid']));
        d4os_io_db_070_set_active('default');
        if (!$result) {
          return;
        }
        $appearance[$i]['Value'] = $result->inventoryID. ":". $asset;
      }
      else if (strpos($appearance[$i]['Name'], '_ap_') !== FALSE) {
        $query = "SELECT assetID FROM {inventoryitems} WHERE inventoryID='%s'";

        d4os_io_db_070_set_active('os_robust');
        $result = db_fetch_object(db_query($query, $appearance[$i]['Value']));
        d4os_io_db_070_set_active('default');
        if (!$result) {
          return;
        }

        $query = "SELECT inventoryID FROM {inventoryitems} WHERE assetID='%s' AND avatarID='%s'";

        d4os_io_db_070_set_active('os_robust');
        $result = db_fetch_object(db_query($query, $result->assetID, $params['avatar_dest_uuid']));
        d4os_io_db_070_set_active('default');
        if (!$result) {
          return;
        }
        $appearance[$i]['Value'] = $result->inventoryID;
      }
      if ($appearance[$i]['Name'] == "UserID") {
        $appearance[$i]['Value'] = $params['avatar_dest_uuid'];
      }
      $queries[] = "('%s', '%s', '%s')";
      $values[] = $params['avatar_dest_uuid'];
      $values[] = $appearance[$i]['Name'];
      $values[] = $appearance[$i]['Value'];
    }
    $query = "INSERT INTO {Avatars} ("
            . "PrincipalID," // 1 str
            . "Name," // 2 str
            . "Value" // 3 str
            . ") VALUES ". implode(',', $queries);
    d4os_io_db_070_set_active('os_robust');
    db_query($query, $values);
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'message' => '',
      'data'    => $params
    );
  }

  function clone_model($params) {
    $params = $this->clone_inventory($params);
    drupal_set_message(t('Inventory folders created and default items added.'));

    $params = $this->clone_appearance($params['data']);
    drupal_set_message(t('Appearance added.'));
  }

  function delete_user_inventory($uuid) {
    // delete attachments and appearance
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {Avatars} WHERE PrincipalID = '%s'", $uuid);
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('Appearance deleted.'));
    // delete folders
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {inventoryfolders} WHERE agentID = '%s'", $uuid);
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('Inventory folders deleted.'));
    // delete items
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {inventoryitems} WHERE avatarID = '%s'", $uuid);
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('Inventory items deleted.'));
  }

}
