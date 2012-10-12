<?php

/**
 * @package   d4os_ui_users
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin panel
 */
function d4os_ui_users_admin() {
  global $base_url;

  // get roles
  $roles = array(
    '0' => t('None'),
  );
  $roles += user_roles(TRUE);

  $form = array();

  /*
   * User level management
   */
  $form['user_level'] = array(
    '#type' => 'fieldset',
    '#title' => t('User level'),
    '#description' => t('That usage is NOT supported by OpenSim devs. Use at your own risk. Side effects unknown. More infos can be found here !link', array('!link' => l('http://opensimulator.org/wiki/Userlevel', 'http://opensimulator.org/wiki/Userlevel'))),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['user_level']['d4os_ui_users_default_user_level'] = array(
    '#type' => 'textfield',
    '#title' => t('Default user level'),
    '#default_value' => variable_get('d4os_ui_users_default_user_level', 0),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t("The default user level for new users."),
    '#required' => TRUE,
  );

  $form['user_level']['d4os_ui_users_user_level_roles_blocked'] = array(
    '#type' => 'select',
    '#title' => t('Blocked'),
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_blocked', 0),
    '#options' => $roles,
    '#description' => t('-1 : Resident:Login blocked'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_0'] = array(
    '#type' => 'select',
    '#title' => 'GOD_NOT',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_0', 0),
    '#options' => $roles,
    '#description' => t('0 : <strong>Resident:GOD_NOT</strong>'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_1'] = array(
    '#type' => 'select',
    '#title' => 'GOD_LIKE',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_1', 0),
    '#options' => $roles,
    '#description' => t('1 : Resident:GOD_LIKE'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_20'] = array(
    '#type' => 'select',
    '#title' => '20',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_20', 0),
    '#options' => $roles,
    '#description' => t('20 : Resident:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_30'] = array(
    '#type' => 'select',
    '#title' => '30',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_30', 0),
    '#options' => $roles,
    '#description' => t('30 : Testing:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_40'] = array(
    '#type' => 'select',
    '#title' => '40',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_40', 0),
    '#options' => $roles,
    '#description' => t('40 : Testing:No payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_50'] = array(
    '#type' => 'select',
    '#title' => '50',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_50', 0),
    '#options' => $roles,
    '#description' => t('50 : Testing:No payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_60'] = array(
    '#type' => 'select',
    '#title' => '60',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_60', 0),
    '#options' => $roles,
    '#description' => t('60 : Member Estatute:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_70'] = array(
    '#type' => 'select',
    '#title' => '70',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_70', 0),
    '#options' => $roles,
    '#description' => t('70 : Member Estatute:Payment info on account'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_80'] = array(
    '#type' => 'select',
    '#title' => '80',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_80', 0),
    '#options' => $roles,
    '#description' => t('80 : Linden Contracted'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_90'] = array(
    '#type' => 'select',
    '#title' => '90',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_90', 0),
    '#options' => $roles,
    '#description' => t('90 : Linden Contracted'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_100'] = array(
    '#type' => 'select',
    '#title' => '100',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_100', 0),
    '#options' => $roles,
    '#description' => t('100 : GOD_CUSTOMER_SERVICE'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_150'] = array(
    '#type' => 'select',
    '#title' => '150',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_150', 0),
    '#options' => $roles,
    '#description' => t('150 : GOD_LIAISON'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_200'] = array(
    '#type' => 'select',
    '#title' => '200',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_200', 0),
    '#options' => $roles,
    '#description' => t('200 : <strong>GOD_FULL</strong>'),
  );
  $form['user_level']['d4os_ui_users_user_level_roles_250'] = array(
    '#type' => 'select',
    '#title' => '250',
    '#default_value' => variable_get('d4os_ui_users_user_level_roles_250', 0),
    '#options' => $roles,
    '#description' => t('250 : GOD_MAINTENANCE'),
  );

  /*
   * Models
   */
  $form['models'] = array(
    '#type' => 'fieldset',
    '#title' => t('Models'),
    '#description' => t('UUID of the users to use as models for new registrations.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['models']['d4os_ui_users_models_image_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Images width'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => variable_get('d4os_ui_users_models_image_width', '150'),
    '#description' => t('Define the width of the model images.'),
  );
  if (module_exists('lightbox2')) {
    $form['models']['d4os_ui_users_models_image_use_lightbox2'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use lightbox2'),
      '#default_value' => variable_get('d4os_ui_users_models_image_use_lightbox2', 0),
      '#description' => t('Select if you want to use the lightbox 2 effect to display images for models.'),
    );
  }
  $model_title = t('Model');
  $model_uuid_title = t('Model uuid');
  $model_name_title = t('Model name');
  $model_picture_url_title = t('Model picture url');
  $default_model_name_value = 'Druplicon';
  $default_model_picture_url_value = $base_url . '/misc/druplicon.png';

  for ($i = 1; $i < 11; $i++) {
    $model_uuid_var_name = 'd4os_ui_users_default_model_uuid_' . $i;
    $model_name_var_name = 'd4os_ui_users_default_model_name_' . $i;
    $model_picture_url_var_name = 'd4os_ui_users_default_model_picture_url_' . $i;
    $form['models'][$i] = array(
      '#type' => 'fieldset',
      '#title' => $model_title . ' ' . $i,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['models'][$i][$model_uuid_var_name] = array(
      '#type' => 'textfield',
      '#title' => $model_uuid_title,
      '#size' => 36,
      '#maxlength' => 36,
      '#default_value' => variable_get($model_uuid_var_name, UUID_ZERO),
    );
    $form['models'][$i][$model_name_var_name] = array(
      '#type' => 'textfield',
      '#title' => $model_name_title,
      '#size' => 25,
      '#maxlength' => 25,
      '#default_value' => variable_get($model_name_var_name, $default_model_name_value),
    );
    $form['models'][$i][$model_picture_url_var_name] = array(
      '#type' => 'textfield',
      '#title' => $model_picture_url_title,
      '#default_value' => variable_get($model_picture_url_var_name, $default_model_picture_url_value),
    );
  }

  /*
   * Grid region owners role
   */
  $form['default_roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default roles'),
    '#description' => t('Assign website roles depending on special actions.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['default_roles']['d4os_ui_users_default_grid_user_role'] = array(
    '#type' => 'select',
    '#title' => t('Grid user role'),
    '#default_value' => variable_get('d4os_ui_users_default_grid_user_role', 0),
    '#options' => $roles,
    '#description' => t('This role is assigned to user when they register on the grid or when a new website account is created from the grid auth.'),
  );
  $form['default_roles']['d4os_ui_users_register_role'] = array(
    '#type' => 'select',
    '#title' => t('Grid register role'),
    '#default_value' => variable_get('d4os_ui_users_register_role', 0),
    '#options' => $roles,
    '#description' => t('This role is allowed to register a grid account for themselves.  This is useful for private grids.  Leave it at "None" to disable this feature.'),
  );
  $form['default_roles']['d4os_ui_users_default_region_owner_role'] = array(
    '#type' => 'select',
    '#title' => t('Region owner role'),
    '#default_value' => variable_get('d4os_ui_users_default_region_owner_role', 0),
    '#options' => $roles,
    '#description' => t('This role is assigned to region owners when running cron.'),
  );

  /*
   * Default home
   */
  $form['home'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Home'),
    '#description' => t('Define a home place for new users.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['home']['d4os_ui_users_default_home_region'] = array(
    '#type' => 'textfield',
    '#title' => t('Home region uuid'),
    '#size' => 36,
    '#maxlength' => 36,
    '#default_value' => variable_get('d4os_ui_users_default_home_region', UUID_ZERO),
  );
  $form['home']['d4os_ui_users_default_home_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Home location'),
    '#size' => 13,
    '#maxlength' => 13,
    '#description' => t('Default value') . ' <128,128,128>',
    '#default_value' => variable_get('d4os_ui_users_default_home_location', '<128,128,128>'),
  );
  $form['home']['d4os_ui_users_default_home_look_at'] = array(
    '#type' => 'textfield',
    '#title' => t('Home look at'),
    '#size' => 13,
    '#maxlength' => 13,
    '#description' => t('Default value') . ' <100,100,100>',
    '#default_value' => variable_get('d4os_ui_users_default_home_look_at', '<100,100,100>'),
  );

  return system_settings_form($form);
}

function d4os_ui_users_admin_validate($form, &$form_state) {
  // check the list of models
  for ($i = 1; $i < 11; ++$i) {
    $model_nbr = 'd4os_ui_users_default_model_uuid_' . $i;
    $model_uuid = $form_state['values'][$model_nbr];
    if ($model_uuid != UUID_ZERO && $model_uuid != '') {
      // check if the uuid is valid
      $d4os_user = D4OS_IO::create('Users');
      $grid_user = $d4os_user->load_user(array('PrincipalID' => $model_uuid));
      $grid_user = $grid_user[0];
      if (!is_object($grid_user)) {
        form_set_error($model_nbr, t('This uuid for model !id is not registered on the grid.', array('!id' => $i)));
        return;
      }
    }
  }
  // check the default region
  $default_region_uuid = $form_state['values']['d4os_ui_users_default_home_region'];
  if ($default_region_uuid != UUID_ZERO && $default_region_uuid != '') {
    $d4os_region = D4OS_IO::create('Regions');
    $region_exists = $d4os_region->region_exists($default_region_uuid);
    if (!$region_exists) {
      form_set_error('d4os_ui_users_default_home_region', t('This uuid for region !uuid is not registered on the grid.', array('!uuid' => $default_region_uuid)));
      return;
    }
  }
}
