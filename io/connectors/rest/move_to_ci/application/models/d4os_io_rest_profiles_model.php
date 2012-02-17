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

class D4OS_IO_rest_Profiles_Model extends CI_Model {
	public $response;
  public $profiles_db;

  function __construct() {
		parent::__construct();
    $this->profiles_db = $this->load->database('profiles', TRUE);
		$this->response = new stdClass;
		$this->response->messages = array();
		$this->response->success = false;
	}

  function get_profile($where) {
    $profiles = array();
		$query = $this->profiles_db->get_where('userprofile', $where);
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $profiles[] = $row;
      }
    }
		if (count($profiles) == 0) {
			$this->response->messages[] = 'profile does not exist';
			return array(
        'success' => FALSE,
        'errorMessage' => $this->response->messages
      );
		}
		else {
			$this->response->success = TRUE;
			$this->response->data = $profiles;
			return array(
        'success' => TRUE,
        'data' => $profiles
      );
		}
	}

  function save_profile($user_profile) {
    if (!is_object($user_profile) || !isset($user_profile['useruuid']) || empty($user_profile['useruuid'])) {
      $this->response->success = FALSE;
      $this->response->messages[] = 'Missing uuid to create a profile.';
    }

    // parse values
    $values = array();
    foreach ($user_profile as $key => $value) {
      if (!empty($value)) {
        switch ($key) {
          case 'useruuid':
            $values['useruuid'] = $value;
            break;
          case 'profileSkillsMask':
            $values['profileSkillsMask'] = $value;
            break;
          case 'profileWantToMask':
            $values['profileWantToMask'] = $value;
            break;
          case 'profileAboutText':
            $values['profileAboutText'] = $value;
            break;
          case 'profileFirstText':
            $values['profileFirstText'] = $value;
            break;
          case 'profileImage':
            $values['profileImage'] = $value;
            break;
          case 'profileFirstImage':
            $values['profileFirstImage'] = $value;
            break;
          case 'profileURL':
            $values['profileURL'] = $value;
            break;
          case 'profilePartner':
            $values['profilePartner'] = $value;
            break;
        }
      }
    }

    // check if profile exists
    $profile_exists = $this->profiles_db->get_where('userprofile', array('useruuid' => $user_profile->user_uuid))->num_rows() > 0;

    // insert or update ?
    if ($profile_exists && count($values)) {
      unset($values['useruuid']);
      $this->profiles_db->update('userprofile', $values, array('useruuid' => $user_profile->user_uuid));
      $return = $this->profiles_db->_error_number()==0;
      if ($return) {
        $this->response->messages[] = 'User profile updated';
      }
    }
    else {
      $this->profiles_db->insert('userprofile', $values);
      $return = $this->profiles_db->_error_number()==0;
      if ($return) {
        $this->response->messages[] = 'User profile added';
      }
    }
  }

  function delete_profile($uuid) {
    // delete classifieds
    $this->profiles_db->delete('classifieds', array('creatoruuid' => $uuid));
		$this->response->messages[] = 'User '. $uuid. ' deleted from classifieds.';
    // delete usernotes
    $this->profiles_db->delete('usernotes', array('useruuid' => $uuid));
		$this->response->messages[] = 'User '. $uuid. ' deleted from usernotes.';
    // delete userpicks
    $this->profiles_db->delete('userpicks', array('creatoruuid' => $uuid));
		$this->response->messages[] = 'User '. $uuid. ' deleted from userpicks.';
    // delete userprofile
    $this->profiles_db->delete('userprofile', array('useruuid' => $uuid));
		$this->response->messages[] = 'User '. $uuid. ' deleted from userprofile.';
    // delete usersettings
    $this->profiles_db->delete('usersettings', array('useruuid' => $uuid));
		$this->response->messages[] = 'User '. $uuid. ' deleted from usersettings.';
    // delete partner
    $this->profiles_db->update('userprofile', array('profilePartner' => ''), array('profilePartner' => $uuid));
    $this->response->messages[] = 'User '. $uuid. ' partner removed from userprofile.';
    return array(
      'success' => TRUE,
      'errorMessage' => $this->response->messages
    );
  }

  /*
  * Profile services
  */
  function avatarclassifiedsrequest($params) {
    $uuid = $params['uuid'];
    $data = array();

    $this->profiles_db->select('*');
    $this->profiles_db->from('classifieds');
    $this->profiles_db->where('creatoruuid', $uuid);
    $query = $this->profiles_db->get();
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $data[] = array(
          "classifiedid"  => $row->classifieduuid,
          "name"          => $row->name
        );
      }
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => $data
    );
  }

  function classified_update($params) {
    $classifieduuid = $params['classifiedUUID'];
    $creator        = $params['creatorUUID'];
    $category       = $params['category'];
    $name           = $params['name'];
    $description    = $params['description'];
    $parceluuid     = $params['parcelUUID'];
    $parentestate   = $params['parentestate'];
    $snapshotuuid   = $params['snapshotUUID'];
    $simname        = $params['sim_name'];
    $globalpos      = $params['globalpos'];
    $parcelname     = $params['parcelname'];
    $classifiedflag = $params['classifiedFlags'];
    $priceforlist   = $params['classifiedPrice'];

    // Check if we already have this one in the database
    $exists = $this->profiles_db->get_where('classifieds', array('classifieduuid' => $classifieduuid))->num_rows() > 0;

    // Doing some late checking
    // Should be done by the module but let's see what happens when
    // I do it here

    if($parcelname == "") {
      $parcelname = "Unknown";
    }

    if($parceluuid == "") {
      $parceluuid = "00000000-0000-0000-0000-0000000000000";
    }

    if($description == "") {
      $description = "No Description";
    }

    if($classifiedflag == 2) {
      $creationdate   = time();
      $expirationdate = time() + (7 * 24 * 60 * 60);
    }
    else {
      $creationdate   = time();
      $expirationdate = time() + (365 * 24 * 60 * 60);
    }

    // fill the values
    $values = array(
      'creatoruuid' => $creator,         // 1
      'creationdate' => $creationdate,    // 2
      'expirationdate' => $expirationdate,  // 3
      'category' => $category,        // 4
      'name' => $name,            // 5
      'description' => $description,     // 6
      'parceluuid' => $parceluuid,      // 7
      'parentestate' => $parentestate,    // 8
      'snapshotuuid' => $snapshotuuid,    // 9
      'simname' => $simname,         // 10
      'posglobal' => $globalpos,       // 11
      'parcelname' => $parcelname,      // 12
      'classifiedflags' => $classifiedflag,  // 13
      'priceforlisting' => $priceforlist,    // 14
      'classifieduuid' => $classifieduuid   // 15
    );

    if (!$exists) {
      $this->profiles_db->insert('classifieds', $values);
    }
    else {
      unset($values['classifieduuid']);
      $this->profiles_db->where('classifieduuid', $classifieduuid);
      $this->profiles_db->insert('classifieds', $values);
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
    );
  }

  function classified_delete($params) {
    $classifieduuid = $params['classifiedID'];
    $this->profiles_db->delete('classifieds', array('classifieduuid' => $classifieduuid));

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => ""
    );
  }

  function avatarpicksrequest($params) {
    $uuid = $params['uuid'];
    $data = array();

    $this->profiles_db->select('*');
    $this->profiles_db->from('userpicks');
    $this->profiles_db->where('creatoruuid', $uuid);
    $query = $this->profiles_db->get();
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $data[] = array(
          "pickid"  => $row->pickuuid,
          "name"    => $row->name
        );
      }
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => $data
    );
  }

  function pickinforequest($params) {
    $uuid = $params['useruuid'];
    $pick = $params['pick_id'];
    $data = array();

    $this->profiles_db->select('*');
    $this->profiles_db->from('userpicks');
    $this->profiles_db->where(array('creatoruuid'=> $uuid, 'pickuuid' => $pick));
    $query = $this->profiles_db->get();
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        if ($row->description == "") {
          $row->description = "No description given";
        }
        $data[] = $row;
      }
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => $data
    );
  }

  function picks_update($params) {
    $pickuuid     = $params['pick_id'];
    $creator      = $params['creator_id'];
    $toppick      = $params['top_pick'];
    $name         = $params['name'];
    $description  = $params['desc'];
    $parceluuid   = $params['parcel_uuid'];
    $snapshotuuid = $params['snapshot_id'];
    $user         = $params['user'];
    $original     = $params['original'];
    $simname      = $params['sim_name'];
    $posglobal    = $params['pos_global'];
    $sortorder    = $params['sort_order'];
    $enabled      = $params['enabled'];

    // Check if we already have this one in the database
    $exists = $this->profiles_db->get_where('userpicks', array('pickuuid' => $pickuuid))->num_rows() > 0;

    // Doing some late checking
    // Should be done by the module but let's see what happens when
    // I do it here

    if($parceluuid == "") {
      $parceluuid = "00000000-0000-0000-0000-0000000000000";
    }

    if($description == "") {
      $description = "No Description";
    }

    if($user == "") {
      $user = "Unknown";
    }

    if($original == "") {
      $original = "Unknown";
    }

    if (!$exists) {
      $values = array(
        'pickuuid' => $pickuuid,      // 1
        'creatoruuid' => $creator,       // 2
        'toppick' => $toppick,       // 3
        'parceluuid' => $parceluuid,    // 4
        'name' => $name,          // 5
        'description' => $description,   // 6
        'snapshotuuid' => $snapshotuuid,  // 7
        'user' => $user,          // 8
        'originalname' => $original,      // 9
        'simname' => $simname,       // 10
        'posglobal' => $posglobal,     // 11
        'sortorder' => $sortorder,     // 12
        'enabled' => $enabled        // 13
      );
      // Create a new record for this avatar
      $this->profiles_db->insert('userpicks', $values);
    }
    else {
      $values = array(
        'parceluuid' => $parceluuid,
        'name' => $name,
        'description' => $description,
        'snapshotuuid' => $snapshotuuid
      );
      $this->profiles_db->where('pickuuid', $pickuuid);
      $this->profiles_db->update('userpicks', $values);
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
    );
  }

  function picks_delete($params) {
    $pickuuid = $params['pick_id'];
    $this->profiles_db->delete('userpicks', array('pickuuid' => $pickuuid));

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
    );
  }

  function avatarnotesrequest($params) {
    $data = array();

    $this->profiles_db->select('*');
    $this->profiles_db->from('usernotes');
    $this->profiles_db->where(array('useruuid' => $params['useruuid'], 'targetuuid' => $params['uuid']));
    $notes = $this->profiles_db->get()->row();

    if ($notes) {
      $data[] = array(
        'useruuid'  => $params['useruuid'],
        'targetid'  => $params['uuid'],
        'notes'     => $notes->notes,
      );
    }
    else {
      $data[] = array(
        'useruuid'  => $params['useruuid'],
        'targetid'  => $params['uuid'],
        'notes'     => '',
      );
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => $data,
    );
  }

  function avatar_notes_update($params) {
    $uuid       = $params['useruuid'];
    $targetuuid = $params['target_id'];
    $notes      = $params['notes'];

    // Check if we already have this one in the database
    $exists = $this->profiles_db->get_where('usernotes', array('useruuid' => $uuid, 'targetuuid' => $targetuuid))->num_rows() > 0;

    if (!$exists) {
      // Create a new record for this avatar note
      $this->profiles_db->insert('usernotes', array('useruuid' => $uuid, 'targetuuid' => $targetuuid, 'notes' => $notes));
    }
    else if ($notes == "") {
      // Delete the record for this avatar note
      $this->profiles_db->delete('usernotes', array('useruuid' => $uuid, 'targetuuid' => $targetuuid));
    }
    else {
      // Update the existing record
      $this->profiles_db->update('usernotes', array('useruuid' => $uuid, 'targetuuid' => $targetuuid, 'notes' => $notes));
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
    );
  }

  function avatar_properties_request($params) {
    $data = array();

    $this->profiles_db->select('*');
    $this->profiles_db->from('userprofile');
    $this->profiles_db->where(array('useruuid'=> $params['useruuid']));
    $query = $this->profiles_db->get();
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $data[] = array(
          "ProfileUrl"          => $row->profileURL, // 1
          "Image"               => $row->profileImage, // 2
          "AboutText"           => $row->profileAboutText, // 3
          "FirstLifeImage"      => $row->profileFirstImage, // 4
          "FirstLifeAboutText"  => $row->profileFirstText, // 5
          "Partner"             => $row->profilePartner, // 6

          //Return interest data along with avatar properties
          "wantmask"   => $row->profileWantToMask, // 7
          "wanttext"   => $row->profileWantToText, // 8
          "skillsmask" => $row->profileSkillsMask, // 9
          "skillstext" => $row->profileSkillsText, // 10
          "languages"  => $row->profileLanguages// 11
        );
      }
    }

    if (count($data) == 0) {
      $data[0]["ProfileUrl"]          = "";
      $data[0]["Image"]               = UUID_ZERO;
      $data[0]["AboutText"]           = "";
      $data[0]["FirstLifeImage"]      = UUID_ZERO;
      $data[0]["FirstLifeAboutText"]  = "";
      $data[0]["Partner"]             = UUID_ZERO;

      $data[0]["wantmask"]   = 0;
      $data[0]["wanttext"]   = "";
      $data[0]["skillsmask"] = 0;
      $data[0]["skillstext"] = "";
      $data[0]["languages"]  = "";
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => $data
    );
  }

  function avatar_properties_update($params) {
    $values = array();
    $values['useruuid']           = isset($params['avatarID']) ?            $params['avatarID'] :           $params['useruuid'];
    $values['profileAboutText']   = isset($params['AboutText']) ?           $params['AboutText'] :          $params['profileAboutText'];
    $values['profileFirstImage']  = isset($params['FirstLifeImage']) ?      $params['FirstLifeImage'] :     $params['profileFirstImage'];
    $values['profileImage']       = isset($params['Image']) ?               $params['Image'] :              $params['profileImage'];
    $values['profileURL']         = isset($params['ProfileUrl'])      ?     $params['ProfileUrl']     :     $params['profileURL'];
    $values['profileFirstText']   = isset($params['FirstLifeAboutText']) ?  $params['FirstLifeAboutText'] : $params['profileFirstText'];
    $values['profilePartner']     = isset($params['Partner']) ?             $params['Partner'] :            $params['profilePartner'];

    // check if the user has a profile
    $profile = $this->profiles_db->get_where('userprofile', array('useruuid' => $params['useruuid']))->num_rows() > 0;

    if (!$profile) {
      $this->profiles_db->insert('userprofile', $values);
    }
    else {
      $useruuid = $values['useruuid'];
      unset($values['useruuid']);
      $this->profiles_db->where('useruuid', $useruuid);
      $this->profiles_db->update('userprofile', $values);
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
    );
  }

  function avatar_interests_request($params) {
    $uuid = $params['useruuid'];
    $data = array();

    $this->profiles_db->select('*');
    $this->profiles_db->from('userprofile');
    $this->profiles_db->where(array('useruuid'=> $uuid));
    $query = $this->profiles_db->get();
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $data[] = array(
          "ProfileUrl" => $row->profileURL
        );
      }
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => $data
    );
  }

  function avatar_interests_update($params) {
    $values = array(
      'profileSkillsMask' => $params['skillsmask'],
      'profileSkillsText' => $params['skillstext'],
      'profileWantToMask' => $params['wantmask'],
      'profileWantToText' => $params['wanttext'],
      'profileLanguages' => $params['languages'],
      'useruuid' => $params['useruuid']
    );

    // check if the user has a profile
    $profile = $this->profiles_db->get_where('userprofile', array('useruuid' => $params['useruuid']))->num_rows() > 0;

    if (!$profile) {

      // create the profile
      $this->profiles_db->insert('userprofile', $values);
    }
    else {
      unset($values['useruuid']);
      $this->profiles_db->where('useruuid', $params['useruuid']);
      $this->profiles_db->update('userprofile', $values);
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
    );
  }

  function user_preferences_request($params) {
    $data = array();
    $this->profiles_db->select('*');
    $this->profiles_db->from('usersettings');
    $this->profiles_db->where(array('useruuid'=> $params['useruuid']));
    $query = $this->profiles_db->get();
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
        $data[] = $row;
      }
    }

    if (count($data) == 0) {
      $data[] = array(
        'imviaemail'  => 0,
        'visible'     => 0,
        'email'       => "email"
      );
    }

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
      'data'          => $data
    );
  }

  function user_preferences_update($params) {
    $email = isset($params['email']) ? $params['email'] : '';
    $data = array(
      'imviaemail' => $params['imViaEmail'],
      'email' => $email,
      'visible' => $params['visible'],
      'useruuid' => $params['useruuid']
    );

    // Check if we already have this one in the database
    $exists = $this->profiles_db->get_where('usersettings', array('useruuid' => $params['useruuid']))->num_rows() > 0;

    if (!$exists) {
      $this->profiles_db->insert('usersettings', $data);
    }
    else {
      unset($data['useruuid']);
      $this->profiles_db->where('useruuid', $params['useruuid']);
      $this->profiles_db->update('usersettings', $data);
    }

    d4os_io_db_070_set_active('os_profile');
    db_query($query, $data);
    d4os_io_db_070_set_active('default');

    return array(
      'success'       => TRUE,
      'errorMessage'  => "",
    );
  }
}
