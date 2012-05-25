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

class D4OS_IO_db_070_Grid implements D4OS_IO_Grid_Interface {
	public $response;
	public $values;

	function __construct() {
		$this->values = array();
	}

	function ping() {
		$this->response->success = d4os_io_db_070_mysql_is_alive('os_robust', 'Robust');
		return;
	}

	function get_grid_infos() {
    $values = new stdClass();
    $d4os_users = D4OS_IO::create('Users');
    $values->online_now = $d4os_users->get_online_users_count();
    $values->online_last_month = $d4os_users->get_online_users_count(2419200);
    $values->users_count = $d4os_users->get_users_count();
    $d4os_regions = D4OS_IO::create('Regions');
    $values->regions_count = $d4os_regions->get_regions_count();
    return $values;
	}

}
