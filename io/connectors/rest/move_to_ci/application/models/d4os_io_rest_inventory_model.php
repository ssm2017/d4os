<?php
/**
 * @package    d4os_io rest
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

class D4OS_IO_rest_Inventory_Model extends CI_Model {
  public $response;

	function __construct() {
		parent::__construct();
		$this->response = new stdClass;
		$this->response->messages = array();
		$this->response->success = false;
	}

	function create_new_inventory($params) {
		$this->config->load('d4os_io_rest');
    $this->load->helper('d4os_io_rest_helper');
		if (!isset($params['user_uuid'])) return FALSE;

		$user_uuid      = $params['user_uuid'];
		$root_folder   	= isset($params['my_inventory'])    ? $params['my_inventory']   : d4os_io_rest_uuid_create();
		$animations     = isset($params['animations'])      ? $params['animations']     : d4os_io_rest_uuid_create();
		$notecards      = isset($params['notecards'])       ? $params['notecards']      : d4os_io_rest_uuid_create();
		$calling_cards  = isset($params['calling_cards'])   ? $params['calling_cards']  : d4os_io_rest_uuid_create();
		$landmarks      = isset($params['landmarks'])       ? $params['landmarks']      : d4os_io_rest_uuid_create();
		$scripts        = isset($params['scripts'])         ? $params['scripts']        : d4os_io_rest_uuid_create();
		$body_parts     = isset($params['body_parts'])      ? $params['body_parts']     : d4os_io_rest_uuid_create();
		$sounds         = isset($params['sounds'])          ? $params['sounds']         : d4os_io_rest_uuid_create();
		$photo_album    = isset($params['photo_album'])     ? $params['photo_album']    : d4os_io_rest_uuid_create();
		$textures       = isset($params['textures'])        ? $params['textures']       : d4os_io_rest_uuid_create();
		$gestures       = isset($params['gestures'])        ? $params['gestures']       : d4os_io_rest_uuid_create();
		$clothing       = isset($params['clothing'])        ? $params['clothing']       : d4os_io_rest_uuid_create();
		$lost_and_found = isset($params['lost_and_found'])  ? $params['lost_and_found'] : d4os_io_rest_uuid_create();
		$trash          = isset($params['trash'])           ? $params['trash']          : d4os_io_rest_uuid_create();
		$objects        = isset($params['objects'])         ? $params['objects']        : d4os_io_rest_uuid_create();

		$values = array(
			array(
				'foldername' 		=> 'My Inventory',
				'type' 				=> 8,
				'version' 			=> 1,
				'folderID' 			=> $root_folder,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> UUID_ZERO
			),
			array(
				'foldername' 		=> 'Animations',
				'type' 				=> 20,
				'version' 			=> 1,
				'folderID' 			=> $animations,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Notecards',
				'type' 				=> 7,
				'version' 			=> 1,
				'folderID' 			=> $notecards,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Calling Cards',
				'type' 				=> 2,
				'version' 			=> 1,
				'folderID' 			=> $calling_cards,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Landmarks',
				'type' 				=> 3,
				'version' 			=> 1,
				'folderID' 			=> $landmarks,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Scripts',
				'type' 				=> 10,
				'version' 			=> 1,
				'folderID' 			=> $scripts,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Body Parts',
				'type' 				=> 13,
				'version' 			=> 1,
				'folderID' 			=> $body_parts,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Sounds',
				'type' 				=> 1,
				'version' 			=> 1,
				'folderID' 			=> $sounds,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Photo Album',
				'type' 				=> 15,
				'version' 			=> 1,
				'folderID' 			=> $photo_album,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Textures',
				'type' 				=> 0,
				'version' 			=> 1,
				'folderID' 			=> $textures,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Gestures',
				'type' 				=> 21,
				'version' 			=> 1,
				'folderID' 			=> $gestures,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Clothing',
				'type' 				=> 5,
				'version' 			=> 1,
				'folderID' 			=> $clothing,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Lost And Found',
				'type' 				=> 16,
				'version' 			=> 1,
				'folderID' 			=> $lost_and_found,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Trash',
				'type' 				=> 14,
				'version' 			=> 1,
				'folderID' 			=> $trash,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
			array(
				'foldername' 		=> 'Objects',
				'type' 				=> 6,
				'version' 			=> 1,
				'folderID' 			=> $objects,
				'agentID' 			=> $user_uuid,
				'parentFolderID' 	=> $root_folder
			),
		);

		$this->db->insert_batch('inventoryfolders', $values);
	}

	function clone_inventory($params) {
		// delete actual folders and items
		$this->db->delete('inventoryfolders', array('agentID' => $params['avatar_dest_uuid']));
		$this->db->delete('inventoryitems', array('avatarID' => $params['avatar_dest_uuid']));

		// get the folders
		$user_folders = array();
		$query = $this->db->get_where('inventoryfolders', array('agentID' => $params['avatar_src_uuid']));
		foreach ($query->result_array() as $row)
		{
			$user_folders[] = $row;
		}

		// no folder for the user, return an error
		if (count($user_folders) == 0) {
			return array(
		      'success' => FALSE,
		      'message' => 'Could not find folders for this user.'
		      );
		}

		// clone the folders
		$params['user_folders'] = $user_folders;
		$new_folders = $this->clone_folders($params);

		// get the items
		$user_items = array();
		$query = $this->db->get_where('inventoryitems', array('avatarID' => $params['avatar_src_uuid']));
		foreach ($query->result_array() as $row)
		{
			$user_items[] = $row;
		}

		// no item for the user, return an error
		if (count($user_items) == 0) {
			return array(
		      'success' => FALSE,
		      'message' => 'Could not find items for this user.'
		      );
		}

		// clone the items
		$params['user_items']   = $user_items;
		$params = array_merge($params, $new_folders['data']);
		$new_items = $this->clone_items($params);
		$params = array_merge($params, $new_items['data']);

		// build the query for folders
		$data = array();
		foreach ($new_items['data']['new_folders'] as $new_folder) {
			$data[] = array(
				'folderName' 		=> $new_folder['folderName'],
				'type' 				=> $new_folder['type'],
				'version' 			=> $new_folder['version'],
				'folderID' 			=> $new_folder['folderID'],
				'agentID' 			=> $new_folder['agentID'],
				'parentFolderID' 	=> $new_folder['parentFolderID']
			);
		}
		// fill the base with all this data
		$this->db->insert_batch('inventoryfolders', $data);

		// build the query for items
		$data = array();
		foreach ($new_items['data']['new_items'] as $new_item) {
			$data[] = array(
				'assetID'						=> $new_item['assetID'], // 1
				'assetType'						=> $new_item['assetType'], // 2 int
				'inventoryName'					=> $new_item['inventoryName'], // 3
				'inventoryDescription'			=> $new_item['inventoryDescription'], // 4
				'inventoryNextPermissions'		=> $new_item['inventoryNextPermissions'], // 5 int
				'inventoryCurrentPermissions'	=> $new_item['inventoryCurrentPermissions'], // 6 int
				'invType'						=> $new_item['invType'], // 7 int
				'creatorID'						=> $new_item['creatorID'], // 8
				'inventoryBasePermissions'		=> $new_item['inventoryBasePermissions'], // 9 int
				'inventoryEveryOnePermissions'	=> $new_item['inventoryEveryOnePermissions'], // 10 int
				'salePrice'						=> $new_item['salePrice'], // 11 int
				'saleType'						=> $new_item['saleType'], // 12 int
				'creationDate'					=> $new_item['creationDate'], // 13 int
				'groupID'						=> $new_item['groupID'], // 14
				'groupOwned'					=> $new_item['groupOwned'], // 15 int
				'flags'							=> $new_item['flags'], // 16 int
				'inventoryID'					=> $new_item['inventoryID'], // 17
				'avatarID'						=> $new_item['avatarID'], // 18
				'parentFolderID'				=> $new_item['parentFolderID'], // 19
				'inventoryGroupPermissions'		=> $new_item['inventoryGroupPermissions'], // 20 int
			);

		}
		// fill the base with all this data
		$this->db->insert_batch('inventoryitems', $data);

		return array(
		    'success' => TRUE,
		    'message' => 'Inventory cloned',
		    'data' => $params
		);
	}

	function clone_appearance($params) {
		// delete actual appearance
		$this->db->delete('Avatars', array('PrincipalID' => $params['avatar_dest_uuid']));

		// get the folders
		$appearance = array();
		$this->db->select('Name, Value');
		$query = $this->db->get_where('Avatars', array('PrincipalID' => $params['avatar_src_uuid']));
		foreach ($query->result_array() as $row) {
			$appearance[] = $row;
		}

		// no items for the user, return an error
		if (count($appearance) == 0) {
			return array(
		      'success' => FALSE,
		      'message' => 'Could not find appearance for this user.'
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

				$this->db->select('inventoryID');
				$query = $this->db->get_where('inventoryitems', array('assetID' => $asset, 'avatarID' => $params['avatar_dest_uuid']));
				$row = $query->row();
				if (!$row) {
					return;
				}
				$appearance[$i]['Value'] = $row->inventoryID. ":". $asset;
			}
			else if (strpos($appearance[$i]['Name'], '_ap_') !== FALSE) {

				$this->db->select('assetID');
				$query = $this->db->get_where('inventoryitems', array('inventoryID' => $appearance[$i]['Value']));
				$row = $query->row();
				if (!$row) {
					return;
				}

				$this->db->select('inventoryID');
				$query = $this->db->get_where('inventoryitems', array('assetID' => $row->assetID, 'avatarID' => $params['avatar_dest_uuid']));
				$row = $query->row();
				if (!$row) {
					return;
				}

				$appearance[$i]['Value'] = $row->inventoryID;
			}
			if ($appearance[$i]['Name'] == "UserID") {
				$appearance[$i]['Value'] = $params['avatar_dest_uuid'];
			}
			$queries[] = "('%s', '%s', '%s')";
			$values[] = array(
				'PrincipalID' => $params['avatar_dest_uuid'],
				'Name' => $appearance[$i]['Name'],
				'Value' => $appearance[$i]['Value']
			);
		}
		$this->db->insert_batch('Avatars', $values);

		return array(
		    'success' => TRUE,
		    'message' => 'Appearance cloned',
		    'data'    => $params
		);
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
		$this->load->helper('d4os_io_rest');
		$new_folders_uuids = array();
		foreach($old_folders_uuids as $uuid) {
			$new_folders_uuids[] = d4os_io_rest_uuid_create();
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
		$this->load->helper('d4os_io_rest');
		$new_items_uuids = array();
		foreach($old_items_uuids as $uuid) {
			$new_items_uuids[] = d4os_io_rest_uuid_create();
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

	function delete_user_inventory($uuid) {
		// delete folders
		$this->db->delete('inventoryfolders', array('agentID' => $uuid));
    $this->response->messages[] = 'Folders deleted for user '. $uuid;
		// delete items
		$this->db->delete('inventoryitems', array('avatarID' => $uuid));
    $this->response->messages[] = 'Items deleted for user '. $uuid;
		// delete appearance
		$this->db->delete('Avatars', array('PrincipalID' => $uuid));
    $this->response->messages[] = 'Appearance deleted for user '. $uuid;
	}
}