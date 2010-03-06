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

/*
 * Profile services
 */
function d4os_io_db_os_profile_services_avatarclassifiedsrequest($params) {
  $req  = $params[0];
  $uuid = $req['uuid'];
  $data = array();

  d4os_io_db_set_active('os_profile');
  $result = db_query("SELECT * FROM {classifieds} WHERE creatoruuid = '%s'", array($uuid));

  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "classifiedid"  => $row["classifieduuid"],
      "name"          => $row["name"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_classified_update($params) {
  $req = $params[0];

  $classifieduuid = $req['classifiedUUID'];
  $creator        = $req['creatorUUID'];
  $category       = $req['category'];
  $name           = $req['name'];
  $description    = $req['description'];
  $parceluuid     = $req['parcelUUID'];
  $parentestate   = $req['parentestate'];
  $snapshotuuid   = $req['snapshotUUID'];
  $simname        = $req['sim_name'];
  $globalpos      = $req['globalpos'];
  $parcelname     = $req['parcelname'];
  $classifiedflag = $req['classifiedFlags'];
  $priceforlist   = $req['classifiedPrice'];

  // Check if we already have this one in the database
  d4os_io_db_set_active('os_profile');
  $ready = db_query(db_query("SELECT count(*) from {classifieds} WHERE classifieduuid = '%s'", array($classifieduuid)));
  d4os_io_db_set_active('default');

  if ($ready == 0) {
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
    
    $insertquery = "INSERT INTO {classifieds} VALUES ("
                ."'%s',"  // 1 $classifieduuid
                ."'%s',"  // 2 $creator
                ."%d,"    // 3 $creationdate
                ."%d,"    // 4 $expirationdate
                ."'%s',"  // 5 $category
                ."'%s',"  // 6 $name
                ."'%s',"  // 7 $description
                ."'%s',"  // 8 $parceluuid
                ."%d,"    // 9 $parentestate
                ."'%s',"  // 10 $snapshotuuid
                ."'%s',"  // 11 $simname
                ."'%s',"  // 12 $globalpos
                ."'%s',"  // 13 $parcelname
                ."%d,"    // 14 $classifiedflag
                ."%d"     // 15 $priceforlist
                .")";
    $values = array(
      $classifieduuid,  // 1
      $creator,         // 2
      $creationdate,    // 3
      $expirationdate,  // 4
      $category,        // 5
      $name,            // 6
      $description,     // 7
      $parceluuid,      // 8
      $parentestate,    // 9
      $snapshotuuid,    // 10
      $simname,         // 11
      $globalpos,       // 12
      $parcelname,      // 13
      $classifiedflag,  // 14
      $priceforlist     // 15
    );
    
    // Create a new record for this classified
    d4os_io_db_set_active('os_profile');
    db_query($insertquery, $values);
    d4os_io_db_set_active('default');
  }
  else {
  // TODO : ask to the devs why is this "else" statement here for
  }
  
  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_classified_delete($params) {
  $req = $params[0];
  $classifieduuid = $req['classifiedID'];

  d4os_io_db_set_active('os_profile');
  db_query("DELETE FROM {classifieds} WHERE classifieduuid = '%s'", array($classifieduuid));
  d4os_io_db_set_active('default');

  return array(
  'success'       => TRUE,
  'errorMessage'  => "",
  'data'          => $data
  );
}

function d4os_io_db_os_profile_services_avatarpicksrequest($params) {
  $req = $params[0];
  $uuid = $req['uuid'];
  $data = array();

  d4os_io_db_set_active('os_profile');
  $result = db_query("SELECT * FROM {userpicks} WHERE creatoruuid = '%s'", array($uuid));

  while (($row = db_fetch_array($result))) {
    $data[] = array(
      "pickid"  => $row["pickuuid"],
      "name"    => $row["name"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_pickinforequest($params) {
  $req = $params[0];
  $uuid = $req['avatar_id'];
  $pick = $req['pick_id'];
  $data = array();

  $values = array($uuid, $pick);
  d4os_io_db_set_active('os_profile');
  $result = db_query("SELECT * FROM {userpicks} WHERE creatoruuid = '%s' AND pickuuid = '%s'", $values);

  while (($row = db_fetch_array($result))) {
    if ($row["description"] == "") {
      $row["description"] = "No description given";
    }
  
    $data[] = array(
      "pickuuid"      => $row["pickuuid"],
      "creatoruuid"   => $row["creatoruuid"],
      "toppick"       => $row["toppick"],
      "parceluuid"    => $row["parceluuid"],
      "name"          => $row["name"],
      "description"   => $row["description"],
      "snapshotuuid"  => $row["snapshotuuid"],
      "user"          => $row["user"],
      "originalname"  => $row["originalname"],
      "simname"       => $row["simname"],
      "posglobal"     => $row["posglobal"],
      "sortorder"     => $row["sortorder"],
      "enabled"       => $row["enabled"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_picks_update($params) {
  $req = $params[0];

  $pickuuid     = $req['pick_id'];
  $creator      = $req['creator_id'];
  $toppick      = $req['top_pick'];
  $name         = $req['name'];
  $description  = $req['desc'];
  $parceluuid   = $req['parcel_uuid'];
  $snapshotuuid = $req['snapshot_id']; 
  $user         = $req['user'];
  $original     = $req['original'];
  $simname      = $req['sim_name'];
  $posglobal    = $req['pos_global'];
  $sortorder    = $req['sort_order'];
  $enabled      = $req['enabled'];

  // Check if we already have this one in the database
  d4os_io_db_set_active('os_profile');
  $ready = db_result(db_query("SELECT COUNT(*) FROM {userpicks} WHERE pickuuid = '%s'", array($pickuuid)));
  d4os_io_db_set_active('default');

  if ($ready == 0) {
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
    d4os_io_db_set_active('os_profile');
    db_query($insertquery, $values);
    d4os_io_db_set_active('default');
  }
  else {
    // Doing some late checking
    // Should be done by the module but let's see what happens when
    // I do it here

    if($parceluuid == "") {
      $parceluuid = "00000000-0000-0000-0000-00000000000";
    }

    if($description == "") {
      $description = "Test";
    }

    if($user == "") {
      $user = "Unknown";
    }

    if($original == "") {
      $original = "Unknown";
    }

    // Update the existing record
    d4os_io_db_set_active('os_profile');
    db_query("UPDATE {userpicks} SET parceluuid = '%s' WHERE pickuuid = '%s'", array($parceluuid, $pickuuid));
    d4os_io_db_set_active('default');
    d4os_io_db_set_active('os_profile');
    db_query("UPDATE {userpicks} SET name = '%s' WHERE pickuuid = '%s'", array($name, $pickuuid));
    d4os_io_db_set_active('default');
    d4os_io_db_set_active('os_profile');
    db_query("UPDATE {userpicks} SET description = '%s' WHERE pickuuid = '%s'", array($description,$pickuuid ));
    d4os_io_db_set_active('default');
    d4os_io_db_set_active('os_profile');
    db_query("UPDATE {userpicks} SET snapshotuuid = '%s' WHERE pickuuid = '%s'", array($snapshotuuid, $pickuuid));
    d4os_io_db_set_active('default');
  }
  
  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_picks_delete($params) {
  $req = $params[0];
  $pickuuid = $req['pick_id'];
  d4os_io_db_set_active('os_profile');
  db_query("DELETE FROM userpicks WHERE pickuuid = '%s'", array($pickuuid));
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_avatarnotesrequest($params) {
  $uuid       = $params['uuid'];
  $targetuuid = $params['avatar_id'];
  $data = array();

  d4os_io_db_set_active('os_profile');
  $result = db_query("SELECT * FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", array($uuid, $targetuuid));

  while ($row = db_fetch_array($result)) {
    $data[] = array(
      "targetid"  => $row["targetuuid"],
      "notes"     => $row["notes"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_avatar_notes_update($params) {
  $req = $params[0];

  $uuid       = $req['avatar_id'];
  $targetuuid = $req['target_id'];
  $notes      = $req['notes'];

  // Check if we already have this one in the database
  d4os_io_db_set_active('os_profile');
  $ready = db_result(db_query("SELECT COUNT(*) FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", array($uuid, $targetuuid)));
  d4os_io_db_set_active('default');

  if ($ready == 0) {
    // Create a new record for this avatar note
    d4os_io_db_set_active('os_profile');
    db_query("INSERT INTO {usernotes} VALUES  ('%s', '%s', '%s')", array($uuid, $targetuuid, $notes));
    d4os_io_db_set_active('default');
  }
  else if ($notes == "") {
    // Delete the record for this avatar note
    d4os_io_db_set_active('os_profile');
    db_query("DELETE FROM {usernotes} WHERE useruuid = '%s' AND targetuuid = '%s'", array($uuid, $targetuuid));
    d4os_io_db_set_active('default');
  }
  else {
    // Update the existing record
    d4os_io_db_set_active('os_profile');
    db_query("UPDATE {usernotes} SET notes = '%s' WHERE useruuid = '%s' AND targetuuid = '%s'", array($notes, $uuid, $targetuuid));
    d4os_io_db_set_active('default');
  }

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_avatar_properties_request($params) {
  $req = $params[0];
  $uuid = $req['avatar_id'];
  $data = array();
// TODO : see
  //d4os_io_db_set_active('os_profile');
  //$result = db_query("SELECT profileURL FROM {opensim.users} WHERE UUID = '%s'", array($uuid));
  //
  //while ($row = db_fetch_array($result)) {
  //  $data[] = array(
  //    "ProfileUrl" => $row["profileURL"]
  //  );
  //}
  //d4os_io_db_set_active('default');
$data[]['ProfileUrl'] = "";
  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_avatar_interests_request($params) {
  $req = $params[0];
  $uuid = $req['avatar_id'];
  $data = array();

  d4os_io_db_set_active('os_profile');
  $result = db_query("SELECT * FROM {userprofile} WHERE useruuid = '%s'", array($uuid));
  while ($row = db_fetch_array($result)) {
    $data[] = array(
      "ProfileUrl" => $row["profileURL"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_avatar_interests_update($params) {
  $req = $params[0];

  $uuid       = $req['avatar_id'];
  $skillstext = $req['skillstext'];
  $skillsmask = $req['skillsmask'];
  $languages  = $req['languages'];
  $wanttext   = $req['wanttext'];
  $wantmask   = $req['wantmask'];

  $values = array(
    $skillsmask,
    $skillstext,
    $wantmask,
    $wanttext,
    $languages,
    $uuid
  );
  d4os_io_db_set_active('os_profile');
  $result = db_query("UPDATE {userprofile} SET "
                  . "profileSkillsMask = %d,"
                  . "profileSkillsText = '%s',"
                  . "profileWantToMask = %d,"
                  . "profileWantToText = '%s',"
                  . "profileLanguages = '%s' "
                  . "where useruuid = '%s'", $values);
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_user_preferences_request($params) {
  $req = $params[0];
  $uuid = $req['avatar_id'];
  $data = array();

  d4os_io_db_set_active('os_profile');
  $result = db_query("SELECT imviaemail,visible,email FROM {usersettings} WHERE useruuid = '%s'", array($uuid));

  while ($row = db_fetch_array($result)) {
    $data[] = array(
      "imviaemail"  => $row["imviaemail"],
      "visible"     => $row["visible"],
      "email"       => $row["email"]
    );
  }
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}

function d4os_io_db_os_profile_services_user_preferences_update($params) {
  $req = $params[0];

  $uuid       = $req['avatar_id'];
  $wantim     = $req['imViaEmail'];
  $directory  = $req['visible'];

  d4os_io_db_set_active('os_profile');
  db_query("UPDATE {usersettings} SET imviaemail = '%s', visible = '%s' where useruuid = '%s'", array($wantim, $directory, $uuid));
  d4os_io_db_set_active('default');

  return array(
    'success'       => TRUE,
    'errorMessage'  => "",
    'data'          => $data
  );
}
