<?php

$st = module_load_include('inc', 'gt_custom', 'civi_conn');

/**
 * Custom form for event participation
 * @param array $form empty form
 * @param array $form empty form state
 * @param integer $event_id id of the event
 * @return array $form complete form 
 */

function gt_custom_register_participate_form($form, &$form_state, $event_id) {
  $form['first_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('First name'), 
    '#size' => 60, 
    '#maxlength' => 128, 
    '#required' => TRUE,
    '#attributes' =>array( 
      'placeholder' =>  'Your first name', 
      'required' => "",
    )
  );
  $form['last_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Last name'), 
    '#size' => 60, 
    '#maxlength' => 128, 
    '#required' => TRUE,        
    '#attributes' =>array( 
      'placeholder' => 'Your last name', 
      'required' => "",
    )
  );
  $form['email'] = array(
    '#type' => 'textfield', 
    '#title' => t('Email'), 
    '#size' => 60, 
    '#maxlength' => 128, 
    '#required' => TRUE, 
    '#attributes' =>array( 
      'placeholder' =>  'Your email address', 
      'required' => "",
      'type' => "email"
    )
  );
  $form['event_id'] = array(
    '#type' => 'hidden',
    '#value' => $event_id
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );    
  return $form;
}

/**
 * Validation handler for event participation form
 * @param array $form form element array
 * @param array $form form state element array
 */

function gt_custom_register_participate_form_validate($form, &$form_state) {
  $email = $form_state['values']['email'];
  if (!valid_email_address($email)) {
    form_set_error('submitted][email_address', t('The email address appears to be invalid.'));
  }
}

/**
 * Submit handler for event participation form
 * @param array $form form element array
 * @param array $form form state element array
 */

function gt_custom_register_participate_form_submit($form, &$form_state) {
  $first_name = NULL;
  $last_name = NULL;
  $email = NULL;
  $event_id = NULL;

  if(isset($form_state['values']['first_name'])) {
    $first_name = $form_state['values']['first_name'];
  }
  if(isset($form_state['values']['last_name'])) {
    $last_name = $form_state['values']['last_name'];
  }
  if(isset($form_state['values']['email'])) {
    $email = $form_state['values']['email'];
  }
  if(isset($form_state['values']['event_id'])) {
    $event_id = $form_state['values']['event_id'];
  }
  if(isset($first_name) && isset($last_name) && isset($email) && isset($event_id)) {
    // Create new individual contact
    $param = array(
      'contact_type' => 'Individual',
      'first_name' => $first_name,
      'last_name' => $last_name,
      'email' => $email
    );
    $api = civi_obj();
    // Create new civi contact for participation
    if ($api->Contact->create($param)) {        
      $civi_contact_id = $api->lastResult->values[0]->id;
      // Create an event participate
      $param = array(
        'contact_id' => $civi_contact_id ,
        'event_id' => $event_id,
        'status_id' => 1,
        'role_id' => 1,
      );
      if ($api->Participant->create($param)) {
        if(isset($api->lastResult->values[0]->id)) {
          drupal_set_message('You have successfully registered for this event');
        }
      } else {
        echo $api->errorMsg();
      }
    } else {
      echo $api->errorMsg();
    }
  }
}

/**
 * Custom form for seocial media links in footer
 * @param array $form empty form
 * @param array $form_state empty form state
 * @return array $form complete form 
 */

function gt_custom_social_media_links_form($form, &$form_state) {
  $fb_link = variable_get('facebook_link');
  $twitter_link = variable_get('twitter_link');
  $youtube_link = variable_get('youtube_link');

  $form['contact'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Social media links'), 
    '#weight' => -1, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
  $form['contact']['fb_link'] = array(
    '#type' => 'textfield', 
    '#title' => t('Facebook link'), 
    '#size' => 60, 
    '#maxlength' => 128, 
    '#default_value' => $fb_link, 
    '#attributes' =>array( 
      'placeholder' =>  'Facebook link', 
    ),
    '#description' => t('Please add full URL. Ex: http://facebook.com/abc ')
  );
  $form['contact']['twitter_link'] = array(
    '#type' => 'textfield', 
    '#title' => t('Twitter link'), 
    '#size' => 60, 
    '#maxlength' => 128, 
    '#default_value' => $twitter_link, 
    '#attributes' =>array( 
      'placeholder' => 'Twitter link', 
    ),
    '#description' => t('Please add full URL. Ex: http://twitter.com/abc ')
  );
  $form['contact']['youtube_link'] = array(
    '#type' => 'textfield', 
    '#title' => t('Youtube link'), 
    '#size' => 60, 
    '#maxlength' => 128, 
    '#default_value' => $youtube_link, 
    '#attributes' =>array( 
      'placeholder' => 'Youtube link', 
    ),
    '#description' => t('Please add full URL. Ex: http://youtube.com/abc ')
  );  
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );    
  return $form;
} 

/**
 * Submit handler for social media link form
 * @param array $form form element array
 * @param array $form form state element array
 */

function gt_custom_social_media_links_form_submit($form, &$form_state) {
  variable_set('facebook_link', $form_state['values']['fb_link']);
  variable_set('twitter_link', $form_state['values']['twitter_link']);
  variable_set('youtube_link', $form_state['values']['youtube_link']);
  drupal_set_message('Social media links are saved successfully');
}

/**
 * Custom form for seocial media links in footer
 * @param array $form empty form
 * @param array $form_state empty form state
 * @return array $form complete form 
 */

function gt_custom_archive_timeline_form($form, $form_state) {
  $archive_timeline = variable_get('archive_timeline');

  $form['fieldset'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Timeline for archive'), 
    '#weight' => -1, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
  $form['fieldset']['archive_timeline'] = array(
    '#type' => 'date_popup',
    '#title' => t('Archive Timeline'),
    '#required' => TRUE,
    '#description' => "All events will be archived which are created before above date and time",
    '#datepicker_options' => array('maxDate' => 0),
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#required' => true,  
    '#default_value' => $archive_timeline  
  );
  $form['fieldset']['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );    
  return $form;
}

/**
 * Submit handler for archive timeline form
 * @param array $form form element array
 * @param array $form form state element array
 */

function gt_custom_archive_timeline_form_submit($form, &$form_state) {
  variable_set('archive_timeline', $form_state['values']['archive_timeline']);
  drupal_set_message('Archive timeline is saved successfully');
}

/**
 * Implements hook_form_alter().
 *   @param $form
 *     Nested array of form elements that comprise the form.
 *   @param $form_state
 *     A keyed array containing the current state of the form
 *   @param $form_id
 *     String representing the name of the form itself. Typically this is the name of the function that generated the form.
 */

function gt_custom_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'news_node_form' || $form_id == 'case_study_node_form' || $form_id == 'national_office_node_form' || $form_id == 'resources_node_form'){
      //hide($form['field_archive']);
  }
  if ($form['#id'] == 'views-exposed-form-resources-page' || $form['#id'] == 'views-exposed-form-case-study-page' || $form['#id'] == 'views-exposed-form-Campaigns-page' || $form['#id'] == 'views-exposed-form-NewsandEvents-page'){
      $form['#attributes']['class'][] = 'header-filters';
  }
  if($form_id == 'event_node_form') {              
    $api = civi_obj();
    // Empty all old events 
    db_truncate('civi_events')->execute();
    // Get all events from CiViCRM
    //echo '<pre>'; print_r($api); exit;
    if ($api->Event->get()) {            
      foreach($api->Event->lastResult->values as $key => $value) {                
        // store all latest events
        db_insert('civi_events')
          -> fields(array(
          'civi_id' => $value->id,
          'civi_event_name' => $value->title
          ))
          ->execute();
        $form['field_civi_event']['und']['#options'][$value->id] = $value->title;
        if (arg(2) == 'edit'):
          $nid = $form_state['node']->nid;
          $node = node_load($form_state['node']->nid);
          if ($node->field_civi_event):
            $form['field_civi_event']['und']['#default_value'] = $node->field_civi_event['und'][0]['value'];
          endif;
        endif;
      }
    } else {
        echo $api->errorMsg();
    }
  } else if(strpos($form_id, 'webform_client_form') !== false) {
    
    $form['#submit'][] = 'gt_custom_contact_webform_submit';
    $form['submitted']['your_name']['#title'] = t($form['submitted']['your_name']['#title']);
    $form['submitted']['your_e_mail_address']['#title'] = t($form['submitted']['your_e_mail_address']['#title']);
    $form['submitted']['subject']['#title'] = t($form['submitted']['subject']['#title']);
    $form['submitted']['message']['#title'] = t($form['submitted']['message']['#title']); 
    $form['actions']['submit']['#value'] = t($form['actions']['submit']['#value']); 
  } else if($form_id == 'views_exposed_form' && ($form['#id'] == 'views-exposed-form-resources-page'|| $form['#id'] == 'views-exposed-form-case-study-page' || $form['#id'] == 'views-exposed-form-Campaigns-page' || $form['#id'] == 'views-exposed-form-NewsandEvents-page')) {
    if ($form['#id'] == 'views-exposed-form-resources-page'): $keyword = 'resources'; elseif ($form['#id'] == 'views-exposed-form-case-study-page'): $keyword = 'case studies'; elseif ($form['#id'] == 'views-exposed-form-Campaigns-page'): $keyword = 'campaigns'; elseif ($form['#id'] == 'views-exposed-form-NewsandEvents-page'): $keyword = 'news & events'; endif;
    if (!arg(0) == 'node'):
      $form['#prefix'] = '<p>' . t('To find '.$keyword.', enter a keyword and use the area filter to find results that only apply in your UK country.') . '</p>';
    endif;
    
    $form['combine']['#attributes']['placeholder'] = t('Enter Keyword');
    $form['field_belongs_to_a_country_categ_tid']['#options']['All'] = t('Choose your country');
    $form['field_topic_category_tid']['#options']['All'] = t('Choose a topic');
    $form['submit']['#value'] = t('Search');
    
  }
}

/**
 * Submit handler for webform
 */

function gt_custom_contact_webform_submit() {
  drupal_set_message(t('Thank you, your submission has been received.'));
}
