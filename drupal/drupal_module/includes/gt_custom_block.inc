<?php

/**
 * Implements hook_block_info().
 * @return array $blocks custom blocks
 */

function gt_custom_block_info() {
  $blocks['your_region'] = array(
    'info' => t('Your Region'),
    'cache' => DRUPAL_NO_CACHE,
    'region' => 'preface_first'
  );
  $blocks['latest_resource'] = array(
    'info' => t('Latest Resources'),
    'cache' => DRUPAL_NO_CACHE,
    'region' => 'preface_first'
  );
  $blocks['register_event'] = array(
    'info' => t('Register Event'),
    'cache' => DRUPAL_NO_CACHE,
    'region' => ''
  );
  $blocks['get_support'] = array(
    'info' => t('Get Support'),
    'cache' => DRUPAL_NO_CACHE,
    'region' => 'preface_first'
  );
  $blocks['campaign'] = array(
    'info' => t('Campaign'),
    'cache' => DRUPAL_NO_CACHE,
    'region' => 'preface_first'
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 * @param array $delta block ids
 * @return array $block custom blocks
 */

function gt_custom_block_view($delta = '') {
  switch ($delta) {
    case 'your_region':
      $block['subject'] = t('Your Country');
      $block['content'] = your_region_block();
    break;
    case 'latest_resource':
      $block['subject'] = t('Latest Resources');
      $block['content'] = latest_resource_block(arg());
    break;
    case 'get_support':
      $block['subject'] = t('Get Support');
      $block['content'] = get_support_block();
    break;
    case 'campaign':
      $block['subject'] = t('Campaign');
      $block['content'] = campaign_block();
    break;
    case 'register_event':
      $block['subject'] = t('Register Event');
      $block['content'] = register_event();
    break;
  
  }
  return $block;
}

/**
 * Implements hook_block_configure().
 * @param array $delta block ids
 * @return array $form block forms
 */

function gt_custom_block_configure($delta = '') {
  $form = array();
  switch ($delta) {
    case 'latest_resource':
      $form['latest_resource_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Latest resource text'),
        '#size' => 60,
        '#default_value' => variable_get('latest_resource_text'),
      );
      $form['latest_resource_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Block link'),
        '#size' => 60,
        '#default_value' => variable_get('latest_resource_link'),
        '#description' => t('Please add full URL. Ex: http://mysite/abc')
      );
    break;
    case 'get_support': 
      $form['get_support_image'] = array(
        '#title' => t('Get support image'),
        '#type' => 'managed_file',
        '#default_value' => variable_get('get_support_image'),
        '#upload_location' => 'public://',
        '#upload_validators' => array(
        'file_validate_extensions' => array('gif png jpg jpeg'),
        'file_validate_size' => array(2 * 1024 * 1024),
        ),
        '#theme' => 'gt_thumb_upload',
        '#required' => true,
      );
      $form['get_support_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Block link'),
        '#size' => 60,
        '#default_value' => variable_get('get_support_link'),
        '#description' => t('Please add full URL. Ex: http://mysite/abc')
      );
    break;
    case 'campaign': 
      $form['campaign_image'] = array(
        '#title' => t('campaign image'),
        '#type' => 'managed_file',
        '#default_value' => variable_get('campaign_image'),
        '#upload_location' => 'public://',
        '#upload_validators' => array(
        'file_validate_extensions' => array('gif png jpg jpeg'),
        'file_validate_size' => array(2 * 1024 * 1024),
        ),
        '#theme' => 'gt_thumb_upload',
        '#required' => true,
      );
      $form['campaign_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Block link'),
        '#size' => 60,
        '#default_value' => variable_get('campaign_link'),
        '#description' => t('Please add full URL. Ex: http://mysite/abc')
      );
    break;
  }
  return $form;
}

/**
 * Implements hook_block_save().
 * @param array $delta block ids
 * @param array $edit block form 
 */

function gt_custom_block_save($delta = '', $edit = array()) {
  switch ($delta) {
    case 'latest_resource':   
      //block_variables($edit, 'latest_resource_text');
      //block_variables($edit, 'latest_resource_link');
      break;
    case 'get_support':
      block_variables($edit, 'get_support_image');
      $file = file_load($edit['get_support_image']);
      $file = file_move($file, 'public://', FILE_EXISTS_RENAME);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $block = block_load('gt_custom', $delta);
      file_usage_add($file, 'gt_custom', 'block', $block->bid);
      block_variables($edit, 'get_support_link');
      break;
    case 'campaign':
      block_variables($edit, 'campaign_image');
      $file = file_load($edit['campaign_image']);
      $file = file_move($file, 'public://', FILE_EXISTS_RENAME);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $block = block_load('gt_custom', $delta);
      file_usage_add($file, 'gt_custom', 'block', $block->bid);
      block_variables($edit, 'campaign_link');
      break;
  }
}

/**
 * To set and delete block values for latest resource, get support and campaign blocks
 * @edit array Block submission values
 * @var string variable name 
 */

function block_variables($edit, $var) {
  if(strpos($var, 'text') !== false || strpos($var, 'link') !== false) {
    if(!empty($edit[$var])) {
      variable_set($var, $edit[$var]);
    } else {
      variable_del($var);
    }
  } else if(strpos($var, 'image') !== false) {
    if($edit[$var] != 0) {
      variable_set($var, $edit[$var]);
    } else {
      variable_del($var);
    }
  }
}

/**
 * To display content in country block
 * @return string HTML for country block
 */

function your_region_block() {
  global $language;
  global $base_url;
  $map_img = $base_url . '/' . path_to_theme() . '/images/map.jpg';
  $country_list = taxonomy_get_tree(3);

  $country_html = '<ul class="choose-country">';
  foreach($country_list as $country) {
    $country_html .= '<li value="' . $country->name . '">' . l(t($country->name), drupal_lookup_path('alias', 'taxonomy/term/'. $country->tid)). '</li>';
  }
  $country_html .= '</ul>';

  $data = '<div class="your-region-block-container">';
  $data .= '<img src="' . $map_img  . '" />';
  $data .= '<div class="choose-region"><span>'.t('Choose your region'). '</span></div><div class="choose-country-outer">' . $country_html . '</div>';
  $data .= '</div>';

  return $data;
}

/**
 * To display content in latest resource block
 * @return string HTML for latest resource block
 */

function latest_resource_block($arg) {
  
  $title = db_select('block', 'b')
    -> fields('b', array('title'))
    -> condition('delta', 'latest_resource')
    -> execute()
    ->fetchField(0); 
  $data = "";
  $text = variable_get('latest_resource_text');
  if (isset($arg[1]) && $arg[1] == 'term'):
      $term = taxonomy_term_load($arg[2]);
      if ($term->vocabulary_machine_name == 'country'):
          $nodes = _get_resources(4,$term->tid);
    endif;  
  elseif (isset($arg[1]) && ($arg[1] == 107 || $arg[1] == 108) ):
      $nodes = _get_resources(4);
      
  else:
      $nodes = _get_resources(1);
  endif;
  
  if ($title) {
    $data .= '<div class="block-title">' . t($title) . '</div>';
  }
  if (isset($nodes) && !empty($nodes)) {
     $data .= '<div class="block-content">';
     foreach ($nodes as $node):
       $title = (strlen($node->title) > 25) ? drupal_substr($node->title,0,23).'...' : drupal_substr($node->title,0,25) ;
       $data .= l(t($title),'node/'.$node->nid);
     endforeach;
     $data .= '</div>';
  }
  else{
    $data .= '<div class="view-empty">';
    $data .= t('No Resources Found..');
    $data .= '</div>';
  }
  return ($data);
}


function _get_resources($count, $term = NULL){
    global $language;
    if ($term):
        $query = db_select('node', 'n');
        $query-> leftJoin('field_data_field_belongs_to_a_country_categ', 't', 'n.nid = t.entity_id');
        $query-> fields('n', array('nid','created','title','language'))
              -> condition('type','resources')
              -> condition('t.field_belongs_to_a_country_categ_tid',$term)
              -> condition('n.status',1)
              -> orderBy('n.created', 'DESC')
              -> condition('n.language', $language->language)
              -> range(0,$count);
        return $query->execute()->fetchAll();

    else:
    return db_select('node', 'n')
            -> fields('n', array('nid','created','title'))
            -> condition('type','resources')
            -> condition('n.status',1)
            -> orderBy('n.created', 'DESC')
            -> condition('n.language', $language->language)
            -> range(0,$count)
            -> execute()
            -> fetchAll();
    endif;
  
}


/**
 * To display content in get support block
 * @return string HTML get support block
 */

function get_support_block() {
  $title = db_select('block', 'b')
    -> fields('b', array('title'))
    -> condition('delta', 'get_support')
    -> execute()
    ->fetchField(0);
  $data = "";
  $fid = variable_get('get_support_image');
  if($fid) {
      $image_fid = file_load($fid);
    if(arg(1) == 107 || arg(1) == 108) {
      $image_uri = image_style_url('get_support_campaign_sidebar', $image_fid->uri);
    } else {
      $image_uri = image_style_url('get_support', $image_fid->uri);
    }
    $image_para = array(
      'path' => $image_uri, 
      'alt' => 'Get Support',
      'title' => 'Get Support',
    );
    $image = theme('image', $image_para);
    $data .= '<div class="block-title">' . t($title) . '</div>';
    $data .= '<div class="block-content">' . $image . '</div>';
  }
  $link = variable_get('get_support_link');
  if (!empty($link) && $link != '#'):
      return l($data, $link, array('html' => true));
  else:
      return $data;
  endif;
  
}

/**
 * To display content in Register event link
 * @return string HTML for register event link
 */

function register_event(){
  $arg = arg();
  if ($arg && isset($arg[1])):
    $node = node_load($arg[1]);
    if ($node->type == 'event'):
      if ($node->field_civi_event):
          return l(t('Register Now'), 'https://www.farmgarden.org.uk/civicrm/event/register?id='.$node->field_civi_event['und'][0]['value'] ,array('attributes'=>array('target'=>'blank', 'class'=>'buttRegister')));
      endif;
    else:
    return false;  
    endif;
  endif;
}

/**
 * To display content in campaign block
 * @return string HTML for campaign block
 */

function campaign_block() {
  $title = db_select('block', 'b')
    -> fields('b', array('title'))
    -> condition('delta', 'campaign')
    -> execute()
    -> fetchField(0); 
  $data = "";
  $fid = variable_get('campaign_image');
  if($fid) {
    $image_fid = file_load($fid);
    if(arg(1) == 107 || arg(1) == 108) {
      $image_uri = image_style_url('get_support_campaign_sidebar', $image_fid->uri);
    } else {
      $image_uri = image_style_url('campaign', $image_fid->uri);
    }
    $image_para = array(
      'path' => $image_uri , 
      'alt' => 'Campaign',
      'title' => 'Campaign',
    );
    $image = theme('image', $image_para);
    $data .= '<div class="block-title">' . t($title) . '</div>';
    $data .= '<div class="block-content">' . $image . '</div>';
  }
  $link = variable_get('campaign_link');
  if (!empty($link) && $link != '#'):
      return l($data, $link, array('html' => true));
  else:
      return $data;
  endif;
}
