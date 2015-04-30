;;; picellif.el --- send lists of files to Windows clipboard

;; Copyright (C) 2015 Rory Yorke

;; Author: Rory Yorke <rory.yorke@gmail.com>
;; Version: 0.1
;; Package-Requires: 
;; Keywords: clipboard
;; URL: http://github.com/roryyorke/picellif

;;; Commentary:

;; This package uses the picellif binary to send lists of files to the
;; Windows clipboard.  This can be useful for pasting into other applications,
;; e.g., for attaching files in Microsoft Outlook messages.

(defun picellif-dired-marked-files ()
  "Send marked files (or current file, if none marked) in current
Dired buffer to picellif."
  (interactive)
  (apply 'call-process "picellif" nil nil nil
         (dired-get-marked-files)))

(defun picellif-buffer-file ()
  "Send file associated with current buffer to picellif."
  (interactive)
  (when (not (buffer-file-name))
    (error "Buffer has no associated file."))
  (call-process "picellif" nil nil nil (buffer-file-name)))
