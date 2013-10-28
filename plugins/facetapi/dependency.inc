<?php

/**
 * @file
/**
 * Basically a dummy Dependency Plugin to hook into the 
 */
class FacetapiDependencyPanelsSearch extends FacetapiDependency {

  protected $defaultSettings;

  /**
   * Executes the dependency check.
   */
  public function execute() {
    // Otherwise hide this facet.
    return NULL;
  }

  /**
   * Adds dependency settings to the form.
   */
  public function settingsForm(&$form, &$form_state) {
    $form[$this->id]['panels_search'] = array(
      '#type' => 'checkbox',
      '#default_value' => TRUE,
      '#title' => t('Panels Search Dependency (always checked)'),
      '#suffix' => t('This dependency will always execute'),
    );
  }
}
