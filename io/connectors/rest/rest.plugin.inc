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
define('D4OS_IO_REST_URL', variable_get('d4os_io_rest_url', 'http://127.0.0.1/rest'));

function d4os_io_rest_http_request($url, $options = NULL) {
  if (!is_array($options)) {
    $options = array();
  }
  $options['timeout'] = 5;

  if (variable_get('d4os_io_rest_security_local_ssl_cert_enable', 0)) {
    //create a stream context for our SSL settings
    $context = stream_context_create();
    //Setup the SSL Options
    stream_context_set_option($context, 'ssl', 'local_cert', variable_get('d4os_io_rest_security_local_ssl_cert_pem_file_path', ''));
    stream_context_set_option($context, 'ssl', 'passphrase', variable_get('d4os_io_rest_security_local_ssl_cert_pem_file_password', ''));
    stream_context_set_option($context, 'ssl', 'allow_self_signed', true);
    stream_context_set_option($context, 'ssl', 'verify_peer', false);
    $options['context'] = $context;
  }
  return drupal_http_request($url, $options);
}