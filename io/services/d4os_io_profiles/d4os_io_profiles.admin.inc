<?php
/**
 * @package    d4os_io_profiles
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
function d4os_io_profiles_admin() {
  $form['user_registration'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('User registration'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['user_registration']['d4os_io_profiles_create_profile_on_register'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create profile on new registration'),
    '#description' => t('Select if you would like to create a new user profile on new user grid registration.'),
    '#default_value' => variable_get('d4os_io_profiles_create_profile_on_register', 1),
  );
  // profile default texts
  $form['user_registration']['profile_texts'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Profile texts'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['user_registration']['profile_texts']['d4os_io_profiles_default_profile_about_text'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Profile about text'),
    '#default_value'  => variable_get('d4os_io_profiles_default_profile_about_text', 'I am a new user'),
    '#required'       => TRUE
  );
  $form['user_registration']['profile_texts']['d4os_io_profiles_default_profile_first_text'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Profile first life tab text'),
    '#default_value'  => variable_get('d4os_io_profiles_default_profile_first_text', 'I am as real as you are'),
    '#required'       => TRUE
  );
  // profile default images
  $form['user_registration']['profile_images'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Profile images'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['user_registration']['profile_images']['d4os_io_profiles_default_profile_image'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default image profile uuid'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_profiles_default_profile_image',UUID_ZERO),
  );
  $form['user_registration']['profile_images']['d4os_io_profiles_default_profile_first_image'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default first life tab image profile uuid'),
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_io_profiles_default_profile_first_image',UUID_ZERO),
  );
  // profile UserTitle
  $form['user_title'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('User title'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $roles = array(
    '0' => t('None'),
  );
  $roles += user_roles(TRUE);
  $form['user_title']['d4os_io_profiles_usertitle_role'] = array(
    '#type' => 'select',
    '#title' => t('UserTitle role'),
    '#description' => t('Select the role that will be used to set the UserTitle'),
    '#options' => $roles,
    '#default_value' => variable_get('d4os_io_profiles_usertitle_role', 0)
  );

  return system_settings_form($form);
}
