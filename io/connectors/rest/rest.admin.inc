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
if (!defined('D4OS_IO_REST_URL'))
  define('D4OS_IO_REST_URL', variable_get('d4os_io_rest_url', 'http://127.0.0.1/rest'));

function d4os_io_rest_admin(&$form) {
  $form['d4os_io_rest'] = array(
    '#type' => 'fieldset',
    '#title' => 'Rest',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['d4os_io_rest']['d4os_io_rest_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#default_value' => D4OS_IO_REST_URL,
    '#description' => t("Default URI"),
    '#required' => TRUE,
  );
  $form['d4os_io_rest']['d4os_io_rest_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#default_value' => variable_get('d4os_io_rest_login', ''),
    '#description' => t("Default login"),
  );
  $form['d4os_io_rest']['d4os_io_rest_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Default password'),
    '#default_value' => variable_get('d4os_io_rest_password', ''),
    '#description' => t("Default password"),
  );
}