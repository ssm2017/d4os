<?php
/**
* @package		d4os
* @copyright	Copyright (C) 2010 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
* @license		GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
* D4os is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
*/

function d4os_ui_users_admin() {
  $form = array();

  $form['d4os_ui_users_profile_texts'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Profile texts'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );

  $form['d4os_ui_users_profile_texts']['d4os_ui_users_default_profile_about_text'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Profile about text'),
    '#default_value'  => variable_get('d4os_ui_users_default_profile_about_text', 'I am a new user'),
    '#required'       => TRUE
  );

  $form['d4os_ui_users_profile_texts']['d4os_ui_users_default_profile_first_text'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Profile first text'),
    '#default_value'  => variable_get('d4os_ui_users_default_profile_first_text', 'Stuf.'),
    '#required'       => TRUE
  );

  $form['d4os_ui_users_god_level'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('God level'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
// TODO : show a message to the users if they are not allowed to log in the grid
  $form['d4os_ui_users_god_level']['d4os_ui_users_default_god_level'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default god level'),
    '#default_value'  => variable_get('d4os_ui_users_default_god_level', 10),
    '#size'           => 3,
    '#maxlength'      => 3,
    '#description'    => t("The default god level for new users."),
    '#required'       => TRUE,
  );

  // get roles
  $roles = array(
    '0' => t('None'),
  );
  $roles = array_merge($roles, user_roles(TRUE));
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_0'] = array(
    '#type'           => 'select',
    '#title'          => t('Blocked'),
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_0',0),
    '#options'        => $roles,
    '#description'    => t('0 : Resident:Login blocked'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_10'] = array(
    '#type'           => 'select',
    '#title'          => '10',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_10',2),
    '#options'        => $roles,
    '#description'    => t('10 : Resident:Normal user'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_20'] = array(
    '#type'           => 'select',
    '#title'          => '20',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_20',0),
    '#options'        => $roles,
    //'#description'    => t('20 : Resident:Payment info on account'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_30'] = array(
    '#type'           => 'select',
    '#title'          => '30',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_30',0),
    '#options'        => $roles,
    //'#description'    => t('30 : Testing:Payment info on account'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_40'] = array(
    '#type'           => 'select',
    '#title'          => '40',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_40',0),
    '#options'        => $roles,
    //'#description'    => t('40 : Testing:No payment info on account'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_50'] = array(
    '#type'           => 'select',
    '#title'          => '50',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_50',0),
    '#options'        => $roles,
    //'#description'    => t('50 : Testing:No payment info on account'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_60'] = array(
    '#type'           => 'select',
    '#title'          => '60',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_60',0),
    '#options'        => $roles,
    //'#description'    => t('60 : Member Estatute:Payment info on account'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_70'] = array(
    '#type'           => 'select',
    '#title'          => '70',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_70',0),
    '#options'        => $roles,
    //'#description'    => t('70 : Member Estatute:Payment info on account'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_80'] = array(
    '#type'           => 'select',
    '#title'          => '80',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_80',0),
    '#options'        => $roles,
    //'#description'    => t('80 : Linden Contracted'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_90'] = array(
    '#type'           => 'select',
    '#title'          => '90',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_90',0),
    '#options'        => $roles,
    //'#description'    => t('90 : Linden Contracted'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_100'] = array(
    '#type'           => 'select',
    '#title'          => '100',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_100',0),
    '#options'        => $roles,
    //'#description'    => t('100 : Linden Contracted'),
  );
  $form['d4os_ui_users_god_level']['d4os_ui_users_god_level_roles_200'] = array(
    '#type'           => 'select',
    '#title'          => '200',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_200',0),
    '#options'        => $roles,
    '#description'    => t('200 : allow user to get admin level'),
  );
  return system_settings_form($form);
}
