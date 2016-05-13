<?php

/**
 * Implements hook_menu()
 * @return array
 *   returns menus
 */

function gt_custom_menu() {
  $items['admin/config/social-links'] = array(
    'title' => t('Social media links'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('gt_custom_social_media_links_form'),
    'access arguments' => array('social media links'),
  );
  $items['contact-us'] = array(
    'title callback' => 'gt_custom_contact_form_title',
    'page callback' => 'gt_custom_contact_form',
    'access callback' => true,
  );
  $items['admin/config/archive-timeline'] = array(
    'title' => t('Timeline for archive'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('gt_custom_archive_timeline_form'),
    'access callback' => true,
  );
  return $items;
}

function gt_custom_contact_form() {
  return '';
}

function gt_custom_contact_form_title() {
  return t('Contact us');
}


