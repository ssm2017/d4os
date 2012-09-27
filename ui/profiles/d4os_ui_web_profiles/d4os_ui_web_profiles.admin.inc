<?php
/**
 * @package   d4os_ui_web_profiles
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
function d4os_ui_web_profiles_admin() {
  $form = array();

  $form['d4os_ui_web_profiles_enable_profile_tab'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Enable profile tab'),
    '#default_value'  => variable_get('d4os_ui_web_profiles_enable_profile_tab', 0),
    '#description'    => t('Select if you want to enable the profile tab for users.'),
  );
  $form['d4os_ui_web_profiles_disable_drupal_user_view_tab'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Disable Drupal user view tab'),
    '#default_value'  => variable_get('d4os_ui_web_profiles_disable_drupal_user_view_tab', 0),
    '#description'    => t('Select if you want to disable the Drupal user view tab. You need to flush caches after changing this option.'),
  );
  /*$form['d4os_ui_web_profiles_url_iframe_attributes'] = array(
    '#type'           => 'textfield',
    '#title'          => t('URL iframe attributes'),
    '#default_value'  => variable_get('d4os_ui_web_profiles_url_iframe_attributes', 'width="600px" height="400px"'),
    '#description'    => t('Define some optional attributes for the profile URL iframe.'),
  );*/
  $form['d4os_ui_web_profiles_images_width'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Images width'),
    '#size'           => 5,
    '#maxlength'      => 5,
    '#default_value'  => variable_get('d4os_ui_web_profiles_images_width', '150'),
    '#description'    => t('Define the width of the profile images.'),
  );
  if (module_exists('lightbox2')) {
    $form['d4os_ui_web_profiles_images_use_lightbox2'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Use lightbox2'),
      '#default_value'  => variable_get('d4os_ui_web_profiles_images_use_lightbox2', 0),
      '#description'    => t('Select if you want to use the lightbox 2 effect to display images in profile.'),
    );
  }
  $form['#submit'][] = 'd4os_ui_web_profiles_admin_submit';

  return system_settings_form($form);
}

function d4os_ui_web_profiles_admin_submit($form, &$form_state) {
  if ($form_state['values']['d4os_ui_web_profiles_disable_drupal_user_view_tab'] != variable_get('d4os_ui_web_profiles_disable_drupal_user_view_tab', 0)) {
    drupal_set_message(t('The cache menu was rebuilt.'));
    module_invoke('menu', 'rebuild');
  }
}
