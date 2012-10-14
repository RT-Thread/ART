/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  AvrdudeUploader - uploader implementation using avrdude
  Part of the Arduino project - http://www.arduino.cc/

  Copyright (c) 2004-05
  Hernando Barragan

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  
  $Id$
*/

package processing.app.debug;

import processing.app.Base;
import processing.app.Preferences;
import processing.app.Serial;
import processing.app.SerialException;
import static processing.app.I18n._;

import java.io.*;
import java.util.*;
import java.util.zip.*;
import javax.swing.*;
import gnu.io.*;


public class STM32DfuUploader extends Uploader  {
  public STM32DfuUploader() {
	  this.verbose = Preferences.getBoolean("build.verbose");
  }

  public boolean uploadUsingPreferences(String buildPath, String className, boolean usingProgrammer)
  throws RunnerException, SerialException {
    this.verbose = verbose;
    Map<String, String> boardPreferences = Base.getBoardPreferences();

    // if no protocol is specified for this board, assume it lacks a 
    // bootloader and upload using the selected programmer.
    if (usingProgrammer || boardPreferences.get("upload.protocol") == null) {
      String programmer = Preferences.get("programmer");
      Target target = Base.getTarget();

      if (programmer.indexOf(":") != -1) {
        target = Base.targetsTable.get(programmer.substring(0, programmer.indexOf(":")));
        programmer = programmer.substring(programmer.indexOf(":") + 1);
      }

      Collection params = getProgrammerCommands(target, programmer);
      params.add("-s 0x08080000");
      params.add("-D root.bin");
      return dfu(params);
    }

    return uploadViaBootloader(buildPath, className);  
  }
  
  private boolean uploadViaBootloader(String buildPath, String className)
  		throws RunnerException, SerialException {

	  String programmer = Preferences.get("programmer");
	  Target target = Base.getTarget();
	  if (programmer.indexOf(":") != -1) {
		  target = Base.targetsTable.get(programmer.substring(0, programmer.indexOf(":")));
		  programmer = programmer.substring(programmer.indexOf(":") + 1);
	  }

	  return burnRomFS(getProgrammerCommands(target, programmer));
  }
  
  public boolean burnBootloader() throws RunnerException {
    String programmer = Preferences.get("programmer");
    Target target = Base.getTarget();
    if (programmer.indexOf(":") != -1) {
      target = Base.targetsTable.get(programmer.substring(0, programmer.indexOf(":")));
      programmer = programmer.substring(programmer.indexOf(":") + 1);
    }
    
    System.out.println("burn bootloader");
    return burnBootloader(getProgrammerCommands(target, programmer));
  }
  
  private Collection getProgrammerCommands(Target target, String programmer) {
    List params = new ArrayList();
    params.add("-d");
    params.add("0483:DF11");
    params.add("-a");
    params.add("0");

    return params;
  }
  
  protected boolean burnRomFS(Collection params) throws RunnerException {
	  boolean result = true;
	  List burnRomFS = new ArrayList();

	  /* re-build RomFS */
	  result = buildRomFS();
	  if (result != true) return result;

      params.add("-s");
      params.add("0x08080000");
      params.add("-D");
      params.add(Base.getTarget().getFolder().getAbsolutePath() + File.separator + "root.bin");

	  return dfu(params);
  }

  protected boolean burnBootloader(Collection params) throws RunnerException {
	  String bootloader_path;
	  
	  bootloader_path = Base.getTarget().getFolder().getAbsolutePath() + File.separator + "rtthread.bin";
	  try {
		  if ((new File(bootloader_path)).exists()) {
		      params.add("-s");
		      params.add("0x08000000");
		      params.add("-D");
		      params.add(Base.getTarget().getFolder().getAbsolutePath() + File.separator + "rtthread.bin");
	
			  return dfu(params);
		  }
	  } catch (RunnerException e) {
		  System.err.println(bootloader_path + " is missing. Please choose ART board and try again!");
	  }

	  return false;
  }

  public boolean buildRomFS() {
	  String rootPath;
	  boolean result = false;
	  String mkromfsProgramer;

	  /* get mkromfs command */
	  if(Base.isLinux()) {
		  if ((new File(Base.getHardwarePath() + "/tools/" + "mkromfs.py")).exists()) {
			  mkromfsProgramer = Base.getHardwarePath() + "/tools/" + "mkromfs.py";
		  } else {
			  mkromfsProgramer = "mkromfs.py";
		  }
	  }
	  else {
		  mkromfsProgramer = Base.getHardwarePath() + "/tools/bin/" + "mkromfs";
	  }

	  List commandBuildRomFS = new ArrayList(Arrays.asList(new String[] {
			  mkromfsProgramer, 
			  "--binary",
			  "--addr", "0x08080000", 
			  Base.getTarget().getFolder().getAbsolutePath() + File.separator + "root"}));

	  if (verbose || Preferences.getBoolean("upload.verbose")) {
		  /* verbose information */
		  for(int i = 0; i < commandBuildRomFS.size(); i++) {
			  System.out.print(commandBuildRomFS.get(i) + " ");
		  }
		  System.out.println();
	  }

	  try {
		  result = executeCommand(commandBuildRomFS);
	  } catch (RunnerException e) {
		  e.printStackTrace();
	  }

	  return result;
  }

  public boolean dfu(Collection p1, Collection p2) throws RunnerException {
    ArrayList p = new ArrayList(p1);
    p.addAll(p2);
    return dfu(p);
  }
  
  public boolean dfu(Collection params) throws RunnerException {
    List commandDownloader = new ArrayList();
    
    /* get dfu-util command */
    if(Base.isLinux()) {
      if ((new File(Base.getHardwarePath() + "/tools/" + "stm32-dfu")).exists()) {
        commandDownloader.add(Base.getHardwarePath() + "/tools/stm32-dfu/dfu-util");
      } else {
        commandDownloader.add("dfu-util");
      }
    }
    else {
      commandDownloader.add(Base.getHardwarePath() + "/tools/stm32-dfu/dfu-util");
    }

    commandDownloader.addAll(params);
	
    if (verbose || Preferences.getBoolean("upload.verbose")) {
    	/* verbose information */
    	for(int i = 0; i < commandDownloader.size(); i++) {
    		System.out.print(commandDownloader.get(i) + " ");
    	}
    	System.out.println();
    }

    return executeCommand(commandDownloader);
  }

  protected boolean executeCommand(Collection commandDownloader) throws RunnerException {
	  int result=0;

	  try {
		  String[] commandArray = new String[commandDownloader.size()];
		  commandDownloader.toArray(commandArray);

		  Process process = Runtime.getRuntime().exec(commandArray);
		  new MessageSiphon(process.getInputStream(), this);
		  new MessageSiphon(process.getErrorStream(), this);

		  // wait for the process to finish.  if interrupted
		  // before waitFor returns, continue waiting
		  //
		  boolean compiling = true;
		  while (compiling) {
			  try {
				  result = process.waitFor();
				  compiling = false;
			  } catch (InterruptedException intExc) {
			  }
		  } 
		  if(exception!=null) {
			  exception.hideStackTrace();
			  throw exception;   
		  }
		  
		  if (result!=0) return false;
	  } catch (Exception e) {
		  String msg = e.getMessage();
		  e.printStackTrace();
		  result = -1;
	  }

	  if (exception != null) throw exception;

	  if ((result != 0) && (result != 1 )) {
		  exception = new RunnerException(SUPER_BADNESS);
	  }

	  return (result == 0); // ? true : false;
  }
}
