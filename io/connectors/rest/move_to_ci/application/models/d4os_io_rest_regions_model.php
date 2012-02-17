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

class D4OS_IO_rest_Regions_Model extends CI_Model {
	public $response;

  function __construct() {
		parent::__construct();
		$this->response = new stdClass;
		$this->response->messages = array();
		$this->response->success = false;
	}

  function region_exists($uuid) {
    return $this->db->get_where('regions', array('uuid' => $uuid))->num_rows() > 0;
  }

  function get_regions_owners() {
    $owners = array();
    $this->db->distinct();
    $this->db->select('owner_uuid');
		$query = $this->db->get('regions');
		foreach ($query->result() as $row) {
			$owners[] = $row->owner_uuid;
		}
		if (count($owners) == 0) {
			$this->response->messages[] = 'owner does not exist';
			return;
		}
		else {
			$this->response->success = TRUE;
			$this->response->data = $owners;
			return $owners;
		}
	}

  function get_regions_count() {
    $count = $this->db->count_all('regions');
    $this->response->success = TRUE;
    $this->response->data = $count;
    //return $this->db->last_query();
    return $count;
  }

  function get_regions_names() {
    $this->db->select('uuid, regionHandle, regionName');
    $this->db->from('regions');
    $this->db->order_by('regionName');
    $query = $this->db->get();
    $items = array();
    if ($query->num_rows() > 0) {
      foreach ($query->result() as $row) {
          $items[] = $row;
      }
    }
    $this->response->success = TRUE;
    return $items;
  }
}