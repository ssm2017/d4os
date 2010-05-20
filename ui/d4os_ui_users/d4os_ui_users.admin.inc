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

function d4os_ui_users_admin() {

  // get roles
  $roles = array(
    '0' => t('None'),
  );
  $roles += user_roles(TRUE);

  $form = array();

  $form['reg_flow'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Grid registration flow'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['reg_flow']['d4os_ui_users_grid_user_create_flow'] = array(
    '#type'           => 'select',
    '#title'          => t('Grid refistration flow'),
    '#options'        => array(
      0 => t('After account registration'),
      1 => t('After account validation'),
    ),
    '#default_value'  => variable_get('d4os_ui_users_grid_user_create_flow', 0),
    '#description'    => t('Select when the grid account is created in the grid database.'),
  );

  $form['profile_texts'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Profile texts'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['profile_texts']['d4os_ui_users_default_profile_about_text'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Profile about text'),
    '#default_value'  => variable_get('d4os_ui_users_default_profile_about_text', 'I am a new user'),
    '#required'       => TRUE
  );
  $form['profile_texts']['d4os_ui_users_default_profile_first_text'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Profile first text'),
    '#default_value'  => variable_get('d4os_ui_users_default_profile_first_text', 'I am as real as you are'),
    '#required'       => TRUE
  );

  $form['users_synchro'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Users synchronization'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  // email generated
  $description = '<p>'. t('This email address is used when a user from the grid is created on the website and the user doesnt have an email address.'). '</p>'
                .'<p>'. t(' <p>You can use tokens from this list :'). '</p>'
                .'<ul>'
                .'<li>[uuid] '. t('The uuid of the inworld user'). '</li>'
                .'<li>[livesite] '. t('The url of the website'). '</li>'
                .'</ul>';
  $form['users_synchro']['d4os_ui_users_default_missing_email'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default email'),
    '#default_value'  => variable_get('d4os_ui_users_default_missing_email', '[uuid]@[livesite]'),
    '#description'    => $description,
    '#required'       => TRUE
  );
  // default role assigned to non email users
  $form['users_synchro']['d4os_ui_users_default_missing_email_role'] = array(
    '#type'           => 'select',
    '#title'          => t('Assigned role'),
    '#default_value'  => variable_get('d4os_ui_users_default_missing_email_role',0),
    '#options'        => $roles,
    '#description'    => t('This role is assigned to synchronized users without an email address.'),
  );
  

  $form['god_level'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('God level'),
    '#description'  => '<div class="message error">'. t('that usage is NOT supported by OpenSim devs. Use at your own risk. Side effects unknown'). '</div>',
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
// TODO : show a message to the users if they are not allowed to log in the grid
  $form['god_level']['d4os_ui_users_default_god_level'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default god level'),
    '#default_value'  => variable_get('d4os_ui_users_default_god_level', 10),
    '#size'           => 3,
    '#maxlength'      => 3,
    '#description'    => t("The default god level for new users."),
    '#required'       => TRUE,
  );

  $form['god_level']['d4os_ui_users_god_level_roles_0'] = array(
    '#type'           => 'select',
    '#title'          => t('Blocked'),
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_0',0),
    '#options'        => $roles,
    '#description'    => t('0 : Resident:Login blocked'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_10'] = array(
    '#type'           => 'select',
    '#title'          => '10',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_10',2),
    '#options'        => $roles,
    '#description'    => t('10 : Resident:Normal user'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_20'] = array(
    '#type'           => 'select',
    '#title'          => '20',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_20',0),
    '#options'        => $roles,
    //'#description'    => t('20 : Resident:Payment info on account'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_30'] = array(
    '#type'           => 'select',
    '#title'          => '30',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_30',0),
    '#options'        => $roles,
    //'#description'    => t('30 : Testing:Payment info on account'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_40'] = array(
    '#type'           => 'select',
    '#title'          => '40',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_40',0),
    '#options'        => $roles,
    //'#description'    => t('40 : Testing:No payment info on account'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_50'] = array(
    '#type'           => 'select',
    '#title'          => '50',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_50',0),
    '#options'        => $roles,
    //'#description'    => t('50 : Testing:No payment info on account'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_60'] = array(
    '#type'           => 'select',
    '#title'          => '60',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_60',0),
    '#options'        => $roles,
    //'#description'    => t('60 : Member Estatute:Payment info on account'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_70'] = array(
    '#type'           => 'select',
    '#title'          => '70',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_70',0),
    '#options'        => $roles,
    //'#description'    => t('70 : Member Estatute:Payment info on account'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_80'] = array(
    '#type'           => 'select',
    '#title'          => '80',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_80',0),
    '#options'        => $roles,
    //'#description'    => t('80 : Linden Contracted'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_90'] = array(
    '#type'           => 'select',
    '#title'          => '90',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_90',0),
    '#options'        => $roles,
    //'#description'    => t('90 : Linden Contracted'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_100'] = array(
    '#type'           => 'select',
    '#title'          => '100',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_100',0),
    '#options'        => $roles,
    //'#description'    => t('100 : Linden Contracted'),
  );
  $form['god_level']['d4os_ui_users_god_level_roles_200'] = array(
    '#type'           => 'select',
    '#title'          => '200',
    '#default_value'  => variable_get('d4os_ui_users_god_level_roles_200',0),
    '#options'        => $roles,
    '#description'    => t('200 : allow user to get admin level'),
  );
  // models
  $form['models'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Models'),
    '#description'  => t('UUID of the users to use as models for new registrations.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['models']['d4os_ui_users_models_image_width'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Images width'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_ui_users_models_image_width', '150'),
    '#description'    => t('Define the width of the model images.'),
  );
  if (module_exists('lightbox2')) {
    $form['models']['d4os_ui_users_models_image_use_lightbox2'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Use lightbox2'),
      '#default_value'  => variable_get('d4os_ui_users_models_image_use_lightbox2', 0),
      '#description'    => t('Select if you want to use the lightbox 2 effect to display images for models.'),
    );
  }
  $form['models']['d4os_ui_users_default_model_1'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 1',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_1', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_2'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 2',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_2', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_3'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 3',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_3', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_4'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 4',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_4', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_5'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 5',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_5', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_6'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 6',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_6', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_7'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 7',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_7', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_8'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 8',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_8', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_9'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 9',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_9', UUID_ZERO),
  );
  $form['models']['d4os_ui_users_default_model_10'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Model'). ' 10',
    '#size'           => 36,
    '#maxlength'      => 36,
    '#default_value'  => variable_get('d4os_ui_users_default_model_10', UUID_ZERO),
  );
  
  return system_settings_form($form);
}

function d4os_ui_users_admin_validate($form, &$form_state) {
  // check the list of models
  for ($i=1; $i<11; ++$i) {
    $model_nbr = 'd4os_ui_users_default_model_'. $i;
    $model_uuid = $form_state['values'][$model_nbr];
    if ($model_uuid != UUID_ZERO && $model_uuid != '') {
      // check if the uuid is valid
      $grid_user = module_invoke_all('os_user_load', array('UUID' => $model_uuid));
      $grid_user = $grid_user[0];
      if (!is_object($grid_user)) {
        form_set_error($model_nbr, t('This uuid for model !id is not registered on the grid.', array('!id' => $i)));
        return;
      }
    }
  }
}