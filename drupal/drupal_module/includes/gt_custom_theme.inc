<?php

/**
 * Implements hook_theme().
 */

function gt_custom_theme() {
  return array(
    'gt_thumb_upload' => array(
        'render element' => 'element',
  ));
}

 /**
  * To display ccontent in country block
  * @param array $variables
  * @return $output HTML for theme
  */

function theme_gt_thumb_upload($variables) {
  $element = $variables['element'];
  $output = '';
   if (isset($element['#file']->uri)) {
      $output = '<div id="edit-logo-ajax-wrapper"><div class="form-item form-type-managed-file form-item-logo">';
      $output .= '<span class="preview-file"><img src="' . image_style_url('thumbnail', $element['#file']->uri) . '" /></span>';
      $output .= '<span class="preview-text" style="margin:0 10px;">' . l($element['#file']->filename, file_create_url($element['#file']->uri)) . '</span>';
      $output .= '<input type="submit" id="edit-' . $element['#name'] . '-remove-button" name="' . $element['#name'] . '_remove_button" value="Remove" class="form-submit ajax-processed">';
      $output .= '<input type="hidden" name="' . $element['#name'] . '[fid]" value="' . $element['#file']->fid . '"></div></div>';
    }
  return $output;
}