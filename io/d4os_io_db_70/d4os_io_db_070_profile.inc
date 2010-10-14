<?php
/**
 * @package    d4os_io_db
 * @subpackage profile
 * @copyright  Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @license    GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/*
 * The code in this script was inspired and mostly copied from osprofile
 * http://forge.opensimulator.org/gf/project/osprofile
 * Copyright (c)Melanie Thielker (http://opensimulator.org/)
 */

/**
 * Save the profile infos
 */
function d4os_io_db_070_os_profile_save($params) {
  global $user;
  $success = FALSE;
  $user->uuid = $user->uid == 1 ? UUID_ZERO : $user->d4os['grid']['account']['uuid'];

  // get the uuid
  $uid = d4os_io_db_070_os_user_get_uid($params['d4os']['grid']['account']['uuid']);
  // load the account
  $account = user_load($uid);

  switch ($params['op']) {

    case 'about-text':
    case 'first-text':
    case 'image':
    case 'first-image':
    case 'home-region':
      $data = array();
      $data['d4os']['grid'] = d4os_array_merge_recursive_distinct($account->d4os['grid'], $params['d4os']['grid']);
      $saved = user_save($account, $data);
      if ($saved->uid) {
        $success = TRUE;
      }
      break;

    case 'partner':
      // get the accounts
      $names    = explode(' ', $params['d4os']['grid']['profile']['partner']['name']);
      $partner  = d4os_io_db_070_os_user_load(array('username' => $names[0], 'lastname' => $names[1]));
      if (is_object($partner)) {
        $partner_uid            = d4os_io_db_070_os_user_get_uid($partner->UUID);
        $partner_account        = user_load($partner_uid);
        $params['source']       = $account;
        $params['destination']  = $partner_account;
        $result = d4os_ui_profile_send_partner_request($params);
        if ($result['success'] == TRUE) $success = TRUE;
      }
      else {
        drupal_set_message(t('Could not find the user in the grid.'), 'error');
        $success = FALSE;
      }
      break;

    case 'notes':
      $params['avatar_id']  = $user->uuid;
      $params['target_id']  = $params['d4os']['grid']['account']['uuid'];
      $params['notes']      = $params['d4os']['grid']['profile']['notes'];
      $result = d4os_io_db_070_os_profile_services_avatar_notes_update($params);
      if ($result['success'] == TRUE) $success = TRUE;
      break;

    case 'url':
      $params['avatarID']   = $user->uuid;
      $params['profileURL'] = $params['d4os']['grid']['profile']['url'];
      $result = d4os_io_db_070_os_profile_services_update_avatar_properties($params);
      if ($result['success'] == TRUE) $success = TRUE;
      break;

    case 'options':
      // save the account
      $data = array();
      $data['d4os']['params']['profile']['website']['display'] = $params['d4os']['params']['profile']['website']['display'];
      $saved = user_save($account, $data);
      if ($saved->uid) {
        $success = TRUE;
      }
      break;
  }
  if ($success) {
    drupal_set_message(t('Saved'));
  }
  else {
    drupal_set_message(t('An error occured during saving'));
  }
}

function d4os_io_db_070_os_profile_user_delete($uuid) {
  d4os_io_db_070_set_active('os_profile');
  // delete all classifieds
  db_query("DELETE FROM {classifieds} WHERE creatoruuid = '%s'", array($uuid));
  // delete all usernotes
  db_query("DELETE FROM {usernotes} WHERE useruuid = '%s' OR targetuuid = '%s'", array($uuid, $uuid));
  // delete all userpicks
  db_query("DELETE FROM {userpicks} WHERE creatoruuid = '%s'", array($uuid));
  // delete all userprofile
  db_query("DELETE FROM {userprofile} WHERE useruuid = '%s'", array($uuid));
  // delete partner
  db_query("UPDATE {userprofile} SET profilePartner= '' WHERE profilePartner = '%s'", array($uuid));
  // delete settings
  db_query("DELETE FROM {usersettings} WHERE useruuid = '%s'", array($uuid));
  d4os_io_db_070_set_active('default');
}

/*
 * Profile services
 */
function d4os_io_db_070_os_profile_services_avatarclassifiedsrequest($params) {
  $uuid = $params['uuid'];
  $data = array();

  d4os_io_db_070_set_active('os_profile');
  $result = db_query("SELECT * FROM {classifieds} WHERE creatoruuid = '%s'", array($uuid));

  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "classifiedid"  => $row["classifieduuid"],
      "name"          => $row["name"]
    );
  }
  d4os_io_db_070_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_070_os_profile_services_classified_update($params) {
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
  d4os_io_db_070_set_active('os_profile');
  $exists = db_result(db_query("SELECT count(*) FROM {classifieds} WHERE classifieduuid = '%s'", array($classifieduuid)));
  d4os_io_db_070_set_active('default');

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
    $creator,         // 1
    $creationdate,    // 2
    $expirationdate,  // 3
    $category,        // 4
    $name,            // 5
    $description,     // 6
    $parceluuid,      // 7
    $parentestate,    // 8
    $snapshotuuid,    // 9
    $simname,         // 10
    $globalpos,       // 11
    $parcelname,      // 12
    $classifiedflag,  // 13
    $priceforlist,    // 14
    $classifieduuid   // 15
  );

  if (!$exists) {

    $query = "INSERT INTO {classifieds} ("
                . "creatoruuid, creationdate, expirationdate, category, name, description, parceluuid, parentestate, snapshotuuid, simname, posglobal, parcelname, classifiedflags, priceforlisting, classifieduuid"
                . ") VALUES ("
                . "'%s',"  // 1 creatoruuid
                . "%d,"    // 2 creationdate
                . "%d,"    // 3 expirationdate
                . "'%s',"  // 4 category
                . "'%s',"  // 5 name
                . "'%s',"  // 6 description
                . "'%s',"  // 7 parceluuid
                . "%d,"    // 8 parentestate
                . "'%s',"  // 9 snapshotuuid
                . "'%s',"  // 10 simname
                . "'%s',"  // 11 posglobal
                . "'%s',"  // 12 parcelname
                . "%d,"    // 13 classifiedflags
                . "%d,"    // 14 priceforlisting
                . "'%s'"   // 15 classifieduuid
                . ")";
  }
  else {
    $query = "UPDATE {classifieds} SET "
                . "creatoruuid = '%s',"     // 1
                . "creationdate = %d,"      // 2 
                . "expirationdate = %d,"    // 3 
                . "category = '%s',"        // 4 
                . "name = '%s',"            // 5 
                . "description = '%s',"     // 6 
                . "parceluuid = '%s',"      // 7 
                . "parentestate = %d,"      // 8 
                . "snapshotuuid = '%s',"    // 9 
                . "simname = '%s',"         // 10 
                . "posglobal = '%s',"       // 11 
                . "parcelname = '%s',"      // 12 
                . "classifiedflags = %d,"   // 13 
                . "priceforlisting = %d"    // 14 
                . " WHERE classifieduuid = '%s'";
  }
  d4os_io_db_070_set_active('os_profile');
  db_query($query, $values);
  d4os_io_db_070_set_active('default');
  
  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
  );
}

function d4os_io_db_070_os_profile_services_classified_delete($params) {
  $classifieduuid = $params['classifiedID'];

  d4os_io_db_070_set_active('os_profile');
  db_query("DELETE FROM {classifieds} WHERE classifieduuid = '%s'", array($classifieduuid));
  d4os_io_db_070_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_070_os_profile_services_avatarpicksrequest($params) {
  $uuid = $params['uuid'];
  $data = array();

  d4os_io_db_070_set_active('os_profile');
  $result = db_query("SELECT * FROM {userpicks} WHERE creatoruuid = '%s'", array($uuid));

  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "pickid"  => $row["pickuuid"],
      "name"    => $row["name"]
    );
  }
  d4os_io_db_070_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_070_os_profile_services_pickinforequest($params) {
  $uuid = $params['avatar_id'];
  $pick = $params['pick_id'];
  $data = array();

  $values = array($uuid, $pick);
  d4os_io_db_070_set_active('os_profile');
  $result = db_query("SELECT * FROM {userpicks} WHERE creatoruuid = '%s' AND pickuuid = '%s'", $values);
  while (($row = db_fetch_array($result))) {
    if ($row["description"] == "") {
      $row["description"] = "No description given";
    }
    $data[] = $row;
  }
  d4os_io_db_070_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_070_os_profile_services_picks_update($params) {
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
  d4os_io_db_070_set_active('os_profile');
  $exists = db_result(db_query("SELECT COUNT(*) FROM {userpicks} WHERE pickuuid = '%s'", array($pickuuid)));
  d4os_io_db_070_set_active('default');

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
    $insertquery = "INSERT INTO {userpicks} VALUES ("
                  ."'%s'," // 1 $pickuuid
                  ."'%s'," // 2 $creator
                  ."'%s'," // 3 $toppick
                  ."'%s'," // 4 $parceluuid
                  ."'%s'," // 5 $name
                  ."'%s'," // 6 $description
                  ."'%s'," // 7 $snapshotuuid
                  ."'%s'," // 8 $user
                  ."'%s'," // 9 $original
                  ."'%s'," // 10 $simname
                  ."'%s'," // 11 $posglobal
                  ."'%s'," // 12 $sortorder
                  ."'%s'" // 13 $enabled
                  .")";

    $values = array(
      $pickuuid,      // 1
      $creator,       // 2
      $toppick,       // 3
      $parceluuid,    // 4
      $name,          // 5
      $description,   // 6
      $snapshotuuid,  // 7
      $user,          // 8
      $original,      // 9
      $simname,       // 10
      $posglobal,     // 11
      $sortorder,     // 12
      $enabled        // 13
    );

    // Create a new record for this avatar note
    d4os_io_db_070_set_active('os_profile');
    db_query($insertquery, $values);
    d4os_io_db_070_set_active('default');
  }
  else {
    $values = array(
      $parceluuid,
      $name,
      $description,
      $snapshotuuid,
      $pickuuid
    );
    $query = "UPDATE {userpicks} SET "
            . "parceluuid = '%s', "
            . "name = '%s', "
            . "description = '%s', "
            . "snapshotuuid = '%s'"
            . " WHERE pickuuid = '%s'";
    // Update the existing record
    d4os_io_db_070_set_active('os_profile');
    db_query($query, array($parceluuid, $name, $description, $snapshotuuid, $pickuuid));
    d4os_io_db_070_set_active('default');
  }

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
  );
}

function d4os_io_db_070_os_profile_services_picks_delete($params) {
  $pickuuid = $params['pick_id'];
  d4os_io_db_070_set_active('os_profile');
  db_query("DELETE FROM {userpicks} WHERE pickuuid = '%s'", array($pickuuid));
  d4os_io_db_070_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
  );
}

function d4os_io_db_070_os_profile_services_avatarnotesrequest($params) {
  $uuid       = $params['avatar_id'];
  $targetuuid = $params['uuid'];
  $data = array();

  d4os_io_db_070_set_active('os_profile');
  $result = db_query("SELECT * FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", array($uuid, $targetuuid));
  
  while ($row = db_fetch_array($result)) {
    $data[] = array(
      'useruuid'  => $row['useruuid'],
      "targetid"  => $row["targetuuid"],
      "notes"     => $row["notes"],
    );
  }
  d4os_io_db_070_set_active('default');

  if (count($data) == 0) {
    $data[] = NULL;
  }

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data,
  );
}

function d4os_io_db_070_os_profile_services_avatar_notes_update($params) {
  $uuid       = $params['avatar_id'];
  $targetuuid = $params['target_id'];
  $notes      = $params['notes'];

  // Check if we already have this one in the database
  d4os_io_db_070_set_active('os_profile');
  $ready = db_result(db_query("SELECT COUNT(*) FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", array($uuid, $targetuuid)));
  d4os_io_db_070_set_active('default');

  if ($ready == 0) {
    // Create a new record for this avatar note
    d4os_io_db_070_set_active('os_profile');
    db_query("INSERT INTO {usernotes} (useruuid, targetuuid, notes) VALUES  ('%s', '%s', '%s')", array($uuid, $targetuuid, $notes));
    d4os_io_db_070_set_active('default');
  }
  else if ($notes == "") {
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
    'success'       => TRUE,
    'errorMessage'  => "",
  );
}

function d4os_io_db_070_os_profile_services_avatar_properties_request($params) {
  $uuid = $params['avatar_id'];
  $data = array();

  d4os_io_db_070_set_active('os_profile');
  $result = db_query("SELECT * FROM {userprofile} WHERE useruuid = '%s'", $uuid);
  while ($row = db_fetch_array($result)) {
    $data[] = array(
      "ProfileUrl"          => $row["profileURL"], // 1
      "Image"               => $row["profileImage"], // 2
      "AboutText"           => $row["profileAboutText"], // 3
      "FirstLifeImage"      => $row["profileFirstImage"], // 4
      "FirstLifeAboutText"  => $row["profileFirstText"], // 5
      "Partner"             => $row["profilePartner"], // 6

      //Return interest data along with avatar properties
      "wantmask"   => $row["profileWantToMask"], // 7
      "wanttext"   => $row["profileWantToText"], // 8
      "skillsmask" => $row["profileSkillsMask"], // 9
      "skillstext" => $row["profileSkillsText"], // 10
      "languages"  => $row["profileLanguages"]// 11
    );
  }
  d4os_io_db_070_set_active('default');

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

function d4os_io_db_070_os_profile_services_update_avatar_properties($params) {
  $avatarID = $params['avatarID'];

  // check if the user has a profile
  d4os_io_db_070_set_active('os_profile');
  $profile = db_result(db_query("SELECT useruuid FROM {userprofile} WHERE useruuid = '%s'", array($avatarID)));
  d4os_io_db_070_set_active('default');

  if (!$profile) {
    $f = array();
    $s = array();
    $v = array();
    foreach ($params as $key => $value) {
      switch ($key) {
        case 'profilePartner':
        case 'profileURL':
        case 'profileFirstImage':
        case 'profileFirstText':
          $f[] = $key;
          $s[] = "'%s'";
          $v[] = $value;
          break;
      }
    }
    $f[] = 'useruuid';
    $s[] = "'%s'";
    $v[] = $avatarID;

    d4os_io_db_070_set_active('os_profile');
    $result = db_query("INSERT INTO {userprofile} (". implode(',', $f). ") VALUES (". implode(',', $s). ")", $v);
    d4os_io_db_070_set_active('default');
  }
  else {
    $f = array();
    $v = array();
    foreach ($params as $key => $value) {
      switch ($key) {
        case 'profilePartner':
        case 'profileURL':
        case 'profileFirstImage':
        case 'profileFirstText':
          $f[] = $key. "='%s'";
          $v[] = $value;
          break;
      }
    }
    $v[] = $avatarID;

    d4os_io_db_070_set_active('os_profile');
    $result = db_query("UPDATE {userprofile} SET ". implode(',', $f). " WHERE useruuid = '%s'", $v);
    d4os_io_db_070_set_active('default');
  }

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
  );
}

function d4os_io_db_070_os_profile_services_avatar_interests_request($params) {
  $uuid = $params['avatar_id'];
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
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_070_os_profile_services_avatar_interests_update($params) {
  $values = array(
    $params['skillsmask'],
    $params['skillstext'],
    $params['wantmask'],
    $params['wanttext'],
    $params['languages'],
    $params['avatar_id']
  );

  // check if the user has a profile
  d4os_io_db_070_set_active('os_profile');
  $profile = db_result(db_query("SELECT useruuid FROM {userprofile} WHERE useruuid = '%s'", array($params['avatar_id'])));
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
    'success'       => TRUE,
    'errorMessage'  => "",
  );
}

function d4os_io_db_070_os_profile_services_user_preferences_request($params) {
  $uuid = $params['avatar_id'];
  $data = array();

  d4os_io_db_070_set_active('os_profile');
  $result = db_query("SELECT * FROM {usersettings} WHERE useruuid = '%s'", array($uuid));

  while ($row = db_fetch_array($result)) {
    $data[] = $row;
  }
  d4os_io_db_070_set_active('default');

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

function d4os_io_db_070_os_profile_services_user_preferences_update($params) {
  // get the user email address
  $account = module_invoke_all('os_user_load', array('UUID' => $params['avatar_id']));

  $data = array(
    $params['imViaEmail'],
    //'email',
    $account[0]->email,
    $params['visible'],
    $params['avatar_id']
  );

  // Check if we already have this one in the database
  d4os_io_db_070_set_active('os_profile');
  $exists = db_result(db_query("SELECT count(*) FROM {usersettings} WHERE useruuid = '%s'", array($params['avatar_id'])));
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
    'success'       => TRUE,
    'errorMessage'  => "",
  );
}

function d4os_io_db_070_os_profile_is_alive() {
  return array(
    'success' => d4os_io_db_070_mysql_is_alive('os_profile', 'os profile'),
  );
}
