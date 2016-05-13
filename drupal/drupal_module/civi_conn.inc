<?php
/**
 * To fetch events from CiViCRM which is installed on any other server
 */

module_load_include('php', 'gt_custom', 'api/class.api');

  define('CIVI_SERVER','https://farmgarden.org.uk');
  define('CIVI_KEY','Bs83rH53097c2602a34yXce5b18f');
  define('CIVI_API_KEY','bJOX7E4opck3cNEwnOzRHp7s');

// Create connection with CiViCRM

function civi_obj() {
  return  $api = new civicrm_api3 (array ('server' => CIVI_SERVER, 'api_key'=> CIVI_API_KEY, 'key'=> CIVI_KEY));
}

