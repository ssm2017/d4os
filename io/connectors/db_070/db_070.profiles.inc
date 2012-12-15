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
class D4OS_IO_db_070_Profiles implements D4OS_IO_Profiles_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  function ping() {
    $this->response->success = d4os_io_db_070_mysql_is_alive('os_profile', 'Robust');
    return;
  }

  function load_profile($params) {
    d4os_io_db_070_set_active('os_profile');
    $profile = db_query("SELECT * FROM {userprofile} WHERE useruuid = :useruuid", array(':useruuid' => $params['useruuid']))->fetch();
    d4os_io_db_070_set_active('default');
    return $profile;
  }

  function save_profile($user_profile) {
    if (!is_object($user_profile) || !isset($user_profile->useruuid) || empty($user_profile->useruuid)) {
      return;
    }

    // check if the profile already exists
    $profile_exists = FALSE;
    d4os_io_db_070_set_active('os_profile');
    $profile_exists = db_query("SELECT COUNT(useruuid) FROM {userprofile} WHERE useruuid=:useruuid", array(':useruuid' => $user_profile->useruuid))->fetchField();
    d4os_io_db_070_set_active('default');

    // update or insert ?
    if ($profile_exists) {
      $values = array();
      foreach ($user_profile as $key => $value) {
        switch ($key) {
          case 'profileSkillsMask':
          case 'profileWantToMask':
          case 'profileAboutText':
          case 'profileFirstText':
          case 'profileImage':
          case 'profileFirstImage':
          case 'profileURL':
          case 'profilePartner':
            $values[$key] = $value;
            break;
        }
      }
      if (count($values)) {
        d4os_io_db_070_set_active('os_profile');
        db_update('userprofile')
            ->fields($values)
            ->condition('useruuid', $user_profile->useruuid)
            ->execute();
        d4os_io_db_070_set_active('default');
        drupal_set_message(t('User profile updated'));
      }
    }
    else {
      $values = array();
      // parse values
      foreach ($user_profile as $key => $value) {
        switch ($key) {
          case 'useruuid':
          case 'profileSkillsMask':
          case 'profileWantToMask':
          case 'profileAboutText':
          case 'profileFirstText':
          case 'profileImage':
          case 'profileFirstImage':
          case 'profileURL':
          case 'profilePartner':
            $values[$key] = $value;
            break;
        }
      }
      if (count($values)) {
        d4os_io_db_070_set_active('os_profile');
        db_insert('userprofile')
            ->fields($values)
            ->execute();
        d4os_io_db_070_set_active('default');
        drupal_set_message(t('User profile added'));
      }
    }
  }

  function delete_profile($uuid) {
    d4os_io_db_070_set_active('os_profile');
    // delete classifieds
    db_delete('classifieds')->condition('creatoruuid', $uuid)->execute();
    // delete usernotes
    db_delete('usernotes')->condition('useruuid', $uuid)->execute();
    // delete userpicks
    db_delete('userpicks')->condition('creatoruuid', $uuid)->execute();
    // delete userprofile
    db_delete('userprofile')->condition('useruuid', $uuid)->execute();
    // delete partner
    db_delete('userprofile')->condition('profilePartner', $uuid)->execute();
    // delete settings
    db_delete('usersettings')->condition('useruuid', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('Classifieds deleted.'));
    drupal_set_message(t('User notes deleted.'));
    drupal_set_message(t('User picks deleted.'));
    drupal_set_message(t('User profile deleted.'));
    drupal_set_message(t('User partner link deleted.'));
    drupal_set_message(t('User settings deleted.'));
  }

  /*
   * Profile services
   */

  function avatarclassifiedsrequest($params) {
    $uuid = $params['uuid'];
    $data = array();

    d4os_io_db_070_set_active('os_profile');
    $results = db_query("SELECT * FROM {classifieds} WHERE creatoruuid = :creatoruuid", array(':creatoruuid' => $uuid));

    foreach ($results as $result) {
      $data[] = array(
        "classifiedid" => $result->classifieduuid,
        "name" => $result->name
      );
    }
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data
    );
  }

  function classified_update($params) {
    $classifieduuid = $params['classifiedUUID'];
    $creator = $params['creatorUUID'];
    $category = $params['category'];
    $name = $params['name'];
    $description = $params['description'];
    $parceluuid = $params['parcelUUID'];
    $parentestate = $params['parentestate'];
    $snapshotuuid = $params['snapshotUUID'];
    $simname = $params['sim_name'];
    $globalpos = $params['globalpos'];
    $parcelname = $params['parcelname'];
    $classifiedflag = $params['classifiedFlags'];
    $priceforlist = $params['classifiedPrice'];

    // Check if we already have this one in the database
    d4os_io_db_070_set_active('os_profile');
    $exists = db_query("SELECT count(*) FROM {classifieds} WHERE classifieduuid = :classifieduuid", array(':classifieduuid' => $classifieduuid))->fetchField();
    d4os_io_db_070_set_active('default');

    // Doing some late checking
    // Should be done by the module but let's see what happens when
    // I do it here

    if ($parcelname == "") {
      $parcelname = "Unknown";
    }

    if ($parceluuid == "") {
      $parceluuid = UUID_ZERO;
    }

    if ($description == "") {
      $description = "No Description";
    }

    if ($classifiedflag == 2) {
      $creationdate = time();
      $expirationdate = time() + (7 * 24 * 60 * 60);
    }
    else {
      $creationdate = time();
      $expirationdate = time() + (365 * 24 * 60 * 60);
    }

    // fill the values
    $values = array(
      'creatoruuid' => $creator, // 1
      'creationdate' => $creationdate, // 2
      'expirationdate' => $expirationdate, // 3
      'category' => $category, // 4
      'name' => $name, // 5
      'description' => $description, // 6
      'parceluuid' => $parceluuid, // 7
      'parentestate' => $parentestate, // 8
      'snapshotuuid' => $snapshotuuid, // 9
      'simname' => $simname, // 10
      'posglobal' => $globalpos, // 11
      'parcelname' => $parcelname, // 12
      'classifiedflags' => $classifiedflag, // 13
      'priceforlisting' => $priceforlist, // 14
      'classifieduuid' => $classifieduuid   // 15
    );

    if (!$exists) {
      d4os_io_db_070_set_active('os_profile');
      db_insert('classifieds')
          ->fields($values)
          ->execute();
      d4os_io_db_070_set_active('default');
    }
    else {
      unset($values['classifieduuid']);
      d4os_io_db_070_set_active('os_profile');
      db_update('classifieds')
          ->fields($values)
          ->condition('classifieduuid', $classifieduuid)
          ->execute();
      d4os_io_db_070_set_active('default');
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
    );
  }

  function classified_delete($params) {
    $classifieduuid = $params['classifiedID'];

    d4os_io_db_070_set_active('os_profile');
    db_query("DELETE FROM {classifieds} WHERE classifieduuid = :classifieduuid", array(':classifieduuid' => $classifieduuid));
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data
    );
  }

  function avatarpicksrequest($params) {
    $uuid = $params['uuid'];
    $data = array();

    d4os_io_db_070_set_active('os_profile');
    $results = db_query("SELECT * FROM {userpicks} WHERE creatoruuid = :creatoruuid", array(':creatoruuid' => $uuid))->fetchAll();

    foreach ($results as $result) {
      $data[] = array(
        "pickid" => $result->pickuuid,
        "name" => $result->name
      );
    }
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data
    );
  }

  function pickinforequest($params) {
    $data = array();

    d4os_io_db_070_set_active('os_profile');
    $results = db_query("SELECT * FROM {userpicks} WHERE creatoruuid = :creatoruuid AND pickuuid = :pickuuid", array(':creatoruuid' => $params['useruuid'], ':pickuuid' => $params['pick_id']))->fetchAssoc();
    foreach ($results as $result) {
      if ($result['description'] == "") {
        $result["description"] = "No description given";
      }
      $data[] = $result;
    }
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data
    );
  }

  function picks_update($params) {
    $pickuuid = $params['pick_id'];
    $creator = $params['creator_id'];
    $toppick = $params['top_pick'];
    $name = $params['name'];
    $description = $params['desc'];
    $parceluuid = $params['parcel_uuid'];
    $snapshotuuid = $params['snapshot_id'];
    $user = $params['user'];
    $original = $params['original'];
    $simname = $params['sim_name'];
    $posglobal = $params['pos_global'];
    $sortorder = $params['sort_order'];
    $enabled = $params['enabled'];

    // Check if we already have this one in the database
    d4os_io_db_070_set_active('os_profile');
    $exists = db_query("SELECT COUNT(*) FROM {userpicks} WHERE pickuuid = :pickuuid", array(':pickuuid' => $pickuuid))->fetchField();
    d4os_io_db_070_set_active('default');

    // Doing some late checking
    // Should be done by the module but let's see what happens when
    // I do it here

    if ($parceluuid == "") {
      $parceluuid = UUID_ZERO;
    }

    if ($description == "") {
      $description = "No Description";
    }

    if ($user == "") {
      $user = "Unknown";
    }

    if ($original == "") {
      $original = "Unknown";
    }

    if (!$exists) {

      $values = array(
        'pickuuid' => $pickuuid, // 1
        'creatoruuid' => $creator, // 2
        'toppick' => $toppick, // 3
        'parceluuid' => $parceluuid, // 4
        'name' => $name, // 5
        'description' => $description, // 6
        'snapshotuuid' => $snapshotuuid, // 7
        'user' => $user, // 8
        'originalname' => $original, // 9
        'simname' => $simname, // 10
        'posglobal' => $posglobal, // 11
        'sortorder' => $sortorder, // 12
        'enabled' => $enabled        // 13
      );

      // Create a new record for this pick
      d4os_io_db_070_set_active('os_profile');
      db_insert('userpicks')
          ->fields($values)
          ->execute();
      d4os_io_db_070_set_active('default');
    }
    else {
      $values = array(
        'parceluuid' => $parceluuid, // 4
        'name' => $name, // 5
        'description' => $description, // 6
        'snapshotuuid' => $snapshotuuid, // 7
      );

      // Update the existing record
      d4os_io_db_070_set_active('os_profile');
      db_update('userpicks')
          ->fields($values)
          ->condition('pickuuid', $pickuuid)
          ->execute();
      d4os_io_db_070_set_active('default');
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
    );
  }

  function picks_delete($params) {
    $pickuuid = $params['pick_id'];
    d4os_io_db_070_set_active('os_profile');
    db_query("DELETE FROM {userpicks} WHERE pickuuid = :pickuuid", array(':pickuuid' => $pickuuid));
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'errorMessage' => "",
    );
  }

  function avatarnotesrequest($params) {
    $data = array();

    d4os_io_db_070_set_active('os_profile');
    $notes = db_query("SELECT * FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", $params['useruuid'], $params['uuid'])->fetchAssoc();
    d4os_io_db_070_set_active('default');

    if ($notes) {
      $data[] = array(
        'useruuid' => $params['useruuid'],
        'targetid' => $params['uuid'],
        'notes' => $notes['notes'],
      );
    }
    else {
      $data[] = array(
        'useruuid' => $params['useruuid'],
        'targetid' => $params['uuid'],
        'notes' => '',
      );
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data,
    );
  }

  function avatar_notes_update($params) {
    $uuid = $params['useruuid'];
    $targetuuid = $params['target_id'];
    $notes = $params['notes'];

    // Check if we already have this one in the database
    d4os_io_db_070_set_active('os_profile');
    $ready = db_result(db_query("SELECT COUNT(*) FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", $uuid, $targetuuid));
    d4os_io_db_070_set_active('default');

    if ($ready == 0) {
      // Create a new record for this avatar note
      d4os_io_db_070_set_active('os_profile');
      db_query("INSERT INTO {usernotes} (useruuid, targetuuid, notes) VALUES  ('%s', '%s', '%s')", $uuid, $targetuuid, $notes);
      d4os_io_db_070_set_active('default');
    }
    elseif ($notes == "") {
      // Delete the record for this avatar note
      d4os_io_db_070_set_active('os_profile');
      db_query("DELETE FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", array($uuid, $targetuuid));
      d4os_io_db_070_set_active('default');
    }
    else {
      // Update the existing record
      d4os_io_db_070_set_active('os_profile');
      db_query("UPDATE {usernotes} SET notes = '%s' WHERE useruuid = '%s' AND targetuuid = '%s'", array($notes, $uuid, $targetuuid));
      d4os_io_db_070_set_active('default');
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
    );
  }

  function avatar_properties_request($params) {
    $data = array();

    d4os_io_db_070_set_active('os_profile');
    $result = db_query("SELECT * FROM {userprofile} WHERE useruuid = '%s'", $params['useruuid']);
    while ($row = db_fetch_array($result)) {
      $data[] = array(
        "ProfileUrl" => $row["profileURL"], // 1
        "Image" => $row["profileImage"], // 2
        "AboutText" => $row["profileAboutText"], // 3
        "FirstLifeImage" => $row["profileFirstImage"], // 4
        "FirstLifeAboutText" => $row["profileFirstText"], // 5
        "Partner" => $row["profilePartner"], // 6
        //Return interest data along with avatar properties
        "wantmask" => $row["profileWantToMask"], // 7
        "wanttext" => $row["profileWantToText"], // 8
        "skillsmask" => $row["profileSkillsMask"], // 9
        "skillstext" => $row["profileSkillsText"], // 10
        "languages" => $row["profileLanguages"]// 11
      );
    }
    d4os_io_db_070_set_active('default');

    if (count($data) == 0) {
      $data[0]["ProfileUrl"] = "";
      $data[0]["Image"] = UUID_ZERO;
      $data[0]["AboutText"] = "";
      $data[0]["FirstLifeImage"] = UUID_ZERO;
      $data[0]["FirstLifeAboutText"] = "";
      $data[0]["Partner"] = UUID_ZERO;

      $data[0]["wantmask"] = 0;
      $data[0]["wanttext"] = "";
      $data[0]["skillsmask"] = 0;
      $data[0]["skillstext"] = "";
      $data[0]["languages"] = "";
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data
    );
  }

  function avatar_properties_update($params) {
    $params['useruuid'] = isset($params['avatarID']) ? $params['avatarID'] : $params['useruuid'];
    $params['profileAboutText'] = isset($params['AboutText']) ? $params['AboutText'] : $params['profileAboutText'];
    $params['profileFirstImage'] = isset($params['FirstLifeImage']) ? $params['FirstLifeImage'] : $params['profileFirstImage'];
    $params['profileImage'] = isset($params['Image']) ? $params['Image'] : $params['profileImage'];
    $params['profileURL'] = isset($params['ProfileUrl']) ? $params['ProfileUrl'] : $params['profileURL'];
    $params['profileFirstText'] = isset($params['FirstLifeAboutText']) ? $params['FirstLifeAboutText'] : $params['profileFirstText'];
    $params['profilePartner'] = isset($params['Partner']) ? $params['Partner'] : $params['profilePartner'];

    // check if the user has a profile
    d4os_io_db_070_set_active('os_profile');
    $profile = db_fetch_object(db_query("SELECT useruuid FROM {userprofile} WHERE useruuid = '%s'", $params['useruuid']));
    d4os_io_db_070_set_active('default');

    if (!$profile) {
      $f = array();
      $s = array();
      $v = array();
      foreach ($params as $key => $value) {
        switch ($key) {
          case 'profileAboutText':
          case 'profileFirstImage':
          case 'profileImage':
          case 'profileURL':
          case 'profileFirstText':
          case 'profilePartner':
            $f[] = $key;
            $s[] = "'%s'";
            $v[] = $value;
            break;
        }
      }
      $f[] = 'useruuid';
      $s[] = "'%s'";
      $v[] = $params['useruuid'];

      d4os_io_db_070_set_active('os_profile');
      $result = db_query("INSERT INTO {userprofile} (" . implode(',', $f) . ") VALUES (" . implode(',', $s) . ")", $v);
      d4os_io_db_070_set_active('default');
    }
    else {
      $f = array();
      $v = array();
      foreach ($params as $key => $value) {
        switch ($key) {
          case 'profileAboutText':
          case 'profileFirstImage':
          case 'profileImage':
          case 'profileURL':
          case 'profileFirstText':
          case 'profilePartner':
            $f[] = $key . "='%s'";
            $v[] = $value;
            break;
        }
      }
      $v[] = $params['useruuid'];

      d4os_io_db_070_set_active('os_profile');
      $result = db_query("UPDATE {userprofile} SET " . implode(',', $f) . " WHERE useruuid = '%s'", $v);
      d4os_io_db_070_set_active('default');
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
    );
  }

  function avatar_interests_request($params) {
    $uuid = $params['useruuid'];
    $data = array();

    d4os_io_db_070_set_active('os_profile');
    $result = db_query("SELECT * FROM {userprofile} WHERE useruuid = '%s'", array($uuid));
    while ($row = db_fetch_array($result)) {
      $data[] = array(
        "ProfileUrl" => $row["profileURL"]
      );
    }
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data
    );
  }

  function avatar_interests_update($params) {
    $values = array(
      $params['skillsmask'],
      $params['skillstext'],
      $params['wantmask'],
      $params['wanttext'],
      $params['languages'],
      $params['useruuid']
    );

    // check if the user has a profile
    d4os_io_db_070_set_active('os_profile');
    $profile = db_result(db_query("SELECT useruuid FROM {userprofile} WHERE useruuid = '%s'", array($params['useruuid'])));
    d4os_io_db_070_set_active('default');

    if (!$profile) {
      // create the profile
      d4os_io_db_070_set_active('os_profile');
      $result = db_query("INSERT INTO {userprofile} (profileSkillsMask, profileSkillsText, profileWantToMask, profileWantToText, profileLanguages, useruuid) VALUES ("
          . "%d,"     // profileSkillsMask
          . "'%s',"   // profileSkillsText
          . "%d,"     // profileWantToMask
          . "'%s',"   // profileWantToText
          . "'%s', "  // profileLanguages
          . "'%s' "   // useruuid
          . ")", $values);
      d4os_io_db_070_set_active('default');
    }
    else {
      d4os_io_db_070_set_active('os_profile');
      $result = db_query("UPDATE {userprofile} SET "
          . "profileSkillsMask = %d,"
          . "profileSkillsText = '%s',"
          . "profileWantToMask = %d,"
          . "profileWantToText = '%s',"
          . "profileLanguages = '%s' "
          . "where useruuid = '%s'", $values);
      d4os_io_db_070_set_active('default');
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
    );
  }

  function user_preferences_request($params) {
    $data = array();
    $query = "SELECT * FROM {usersettings} WHERE useruuid = '%s'";

    d4os_io_db_070_set_active('os_profile');
    $result = db_query($query, $params['useruuid']);
    while ($row = db_fetch_array($result)) {
      $data[] = $row;
    }
    d4os_io_db_070_set_active('default');

    if (count($data) == 0) {
      $data[] = array(
        'imviaemail' => 0,
        'visible' => 0,
        'email' => "email"
      );
    }

    return array(
      'success' => TRUE,
      'errorMessage' => "",
      'data' => $data
    );
  }

  function user_preferences_update($params) {
    // get the user email address from the website
    $mail = db_result(db_query("SELECT u.mail FROM {users} AS u LEFT JOIN {d4os_ui_users} AS du ON du.uid=u.uid WHERE du.uuid='%s'", $params['useruuid']));
    if ($mail === FALSE) {
      $mail = '';
    }

    $data = array(
      $params['imViaEmail'],
      //'email',
      $mail,
      $params['visible'],
      $params['useruuid']
    );

    // Check if we already have this one in the database
    d4os_io_db_070_set_active('os_profile');
    $exists = db_result(db_query("SELECT count(*) FROM {usersettings} WHERE useruuid = '%s'", array($params['useruuid'])));
    d4os_io_db_070_set_active('default');

    if (!$exists) {
      $query = "INSERT INTO {usersettings} (imviaemail, email, visible, useruuid) VALUES ('%s', '%s', '%s', '%s')";
    }
    else {
      $query = "UPDATE {usersettings} SET imviaemail = '%s', email = '%s', visible = '%s' where useruuid = '%s'";
    }

    d4os_io_db_070_set_active('os_profile');
    db_query($query, $data);
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'errorMessage' => "",
    );
  }

}
