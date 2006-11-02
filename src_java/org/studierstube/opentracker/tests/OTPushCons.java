//Package   : OpenTracker
//pushcons.cc              Created   : 2006/11/1
//Author    : Joseph Newman

//Copyright (C) 2006 Joseph Newman

//This file is part of the OpenTracker framework.


//Description:
//Push Model consumer implementation borrowed from Alex Tingle


package org.studierstube.opentracker.tests;

import gnu.getopt.Getopt;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CosLifeCycle.*;
import org.omg.CosEventComm.*;
import org.omg.CosEventChannelAdmin.*;

import net.sourceforge.omniorb.EventChannelAdmin.*;

import org.studierstube.opentracker.OT_CORBA.*;

abstract public class OTPushCons extends PushConsumerPOA
{
	//
	// class members
	protected int _disconnect =0;
	protected int _count =0;
	protected boolean _disconnectFlag =false;

	public OTPushCons(int disconnect)
	{
		_disconnect=disconnect;
	}

	/** Used to pass a signal from the CORBA message processing thread
	 *  (in push() method) to the main thread. In C++ & Python we use a
	 * condition variable. Java doesn't have such luxury, so we use a
	 * flag (_disconnectFlag) instead.
	 */
	protected synchronized void setDisconnectFlag(boolean value)
	{
		_disconnectFlag=value;
		notifyAll(); // Wakes up threads stuck at wait()
	}

	/** Wait for a signal to disconnect. */
	protected synchronized void waitDisconnect()
	{
		while(_disconnectFlag==false)
		{
			try {
				wait(); // Sleep until notifyAll() is called.
			} catch(InterruptedException ex) { }
		}
	}

	/** The one and only PushCons object */
	static protected OTPushCons consumer =null;

	static ConsumerAdmin consumer_admin =null;
	
	static public OTPushCons getPushCons() {
		return consumer;
	}

	static public ConsumerAdmin getConsumerAdmin(EventChannel channel) {
		//
		// Get Consumer admin interface - retrying on Comms Failure.
		ConsumerAdmin consumer_admin;
		while(true)
		{
			try {
				consumer_admin=channel.for_consumers ();
				if	(consumer_admin==null)
				{	
					System.err.println(
					"Event Channel returned nil Consumer Admin!");
					System.exit (1);
				}
				break;
			}
			catch (COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE exception "
						+"obtaining Consumer Admin! Retrying...");
				continue;
			}
		}
		System.out.println("Obtained ConsumerAdmin.");
		return consumer_admin;

	}

	static public void connectPushConsumer(ProxyPushSupplier proxy_supplier, PushConsumerPOA consumer) {
		//
		// Connect Push Consumer - retrying on Comms Failure.
		while(true)
		{
			try {
				proxy_supplier.connect_push_consumer(consumer._this());
				break;
			}
			catch(BAD_PARAM ex) {
				System.err.println(
				"Caught BAD_PARAM exception connecting Push Consumer!");
				System.exit(1);
			}
			catch(AlreadyConnected ex) {
				System.err.println("Proxy Push Supplier already connected!");
				break;
			}
			catch(TypeError ex) {
				System.err.println(
				"Caught TypeError connecting Push Consumer!");
				break;
			}
			catch (COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE exception "
						+"connecting Push Consumer! Retrying...");
				continue;
			}
		}
		System.out.println("Connected Push Consumer.");
	}

	public static void disconnectPushConsumer(ProxyPushSupplier proxy_supplier) {
		// Disconnect - retrying on Comms Failure.
		while(true)
		{
			try {
				proxy_supplier.disconnect_push_supplier();
				break;
			}
			catch(COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE exception "
						+"disconnecting Push Consumer! Retrying...");
				continue;
			}
		}
		System.out.println("Disconnected Push Consumer.");
	}

	public static ProxyPushSupplier getProxyPushSupplier(ConsumerAdmin consumer_admin2) {
		ProxyPushSupplier proxy_supplier;
		//
		// Get proxy supplier - retrying on Comms Failure.
		while(true)
		{
			try {
				proxy_supplier=consumer_admin.obtain_push_supplier();
				if(proxy_supplier==null)
				{
					System.err.println(
					"Consumer Admin returned nil proxy_supplier!");
					System.exit(1);
				}
				break;
			}
			catch(COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE Exception "
						+"obtaining Push Supplier! Retrying...");
				continue;
			}
		}
		System.out.println("Obtained ProxyPushSupplier.");
		return proxy_supplier;

	}
	
	
	
	public void mainLoop(String args[])
	{
		//
		// Start orb.
		ORB orb =ORB.init(args, null);
		// Must strip out ORB arguments manually in Java,
		// since ORB.init() doesn't do it for us.
		args=stripOrbArgs(args);

		// Process Options
		int discnum =0;
		int sleepInterval =0;
		String channelName ="EventChannel";

		Getopt g =new Getopt("eventc",args,"hd:s:n:");
		int c;
		while ((c = g.getopt()) != -1)
		{
			switch (c)
			{
			case 'd': Integer discnumObj =new Integer(g.getOptarg());
			discnum=discnumObj.intValue();
			break;

			case 's': Integer sleepIntervalObj =new Integer(g.getOptarg());
			sleepInterval=sleepIntervalObj.intValue();
			break;

			case 'n': channelName=g.getOptarg();
			break;

			case 'h':
			default : usage();
			System.exit(-1);
			break;
			}
		}

		EventChannel channel =null;

		String action=""; // Use this variable to help report errors.
		try {

			action="resolve initial reference 'RootPOA'";
			org.omg.CORBA.Object obj =orb.resolve_initial_references("RootPOA");
			POA rootPoa =POAHelper.narrow(obj);
			if(rootPoa==null)
				throw new OBJECT_NOT_EXIST();

			action="activate the PushConsumer";
			//consumer = getPushCons(discnum);
			rootPoa.activate_object(consumer);

			action="activate the RootPOA's POAManager";
			POAManager pman =rootPoa.the_POAManager();
			pman.activate();

			channel = getEventChannel(g, args, orb, channelName);

		}
		catch(org.omg.CORBA.ORBPackage.InvalidName ex) { // resolve_initial_references
			System.err.println("Failed to "+action+". ORB::InvalidName");
			System.exit(1);
		}
		catch(TRANSIENT ex) { // _narrow()
			System.err.println("Failed to "+action+". TRANSIENT");
			System.exit(1);
		}
		catch(OBJECT_NOT_EXIST ex) { // _narrow()
			System.err.println("Failed to "+action+". OBJECT_NOT_EXIST");
			System.exit(1);
		}
		catch (SystemException ex) {
			System.err.println("System exception, unable to "+action);
			System.exit(1);
		}
		catch (UserException ex) {
			System.err.println("CORBA exception, unable to "+action);
			System.exit(1);
		}

		consumer_admin = getConsumerAdmin(channel);

		while(true)
		{
			// Connect and listen for events.
			//getPushCons().connect();
			connect();
			// Disconnected... reconnect!

			// Yawn.
			System.out.println("Sleeping "+sleepInterval+" seconds.");
			try {
				java.lang.Thread.sleep(sleepInterval*1000);
			}
			catch(java.lang.InterruptedException ex) {
				System.err.println("Sleep interrupted.");
				// just carry on...
			}
		}

		// NEVER GET HERE

	} // end main()

	//abstract OTPushCons getPushCons(int discnum);
	
	//abstract OTPushCons getPushCons();

	public static EventChannel getEventChannel(Getopt g, String args[], ORB orb, String channelName) {
		//
		// Obtain object reference to EventChannel
		// (from command-line argument or from the Naming Service).
		String action="";
		EventChannel _channel;
		org.omg.CORBA.Object obj = null;
		try {
			if(g.getOptind()<args.length)
			{
				action="convert URI from command line into object reference";
				obj=orb.string_to_object(args[g.getOptind()]);
			}
			else
			{
				action="resolve initial reference 'NameService'";
				try {
					obj=orb.resolve_initial_references("NameService");
				} catch (org.omg.CORBA.COMM_FAILURE ex) {
					System.err.println("Error occurred whilst trying to contact the NameService. Is it running?");
					System.exit(1);
				} 
				NamingContextExtOperations rootContext=NamingContextExtHelper.narrow(obj);			
				if(rootContext==null)
					throw new OBJECT_NOT_EXIST();

				NameComponent name[] = rootContext.to_name(channelName);

				action="find EventChannel in NameService";
				System.out.println(action);
				obj=rootContext.resolve(name);
			}

			action="narrow object reference to event channel";
			_channel=EventChannelHelper.narrow(obj);
			if(_channel==null)
			{
				System.err.println("Failed to narrow Event Channel reference.");
				System.exit(1);
			}
			return _channel;
		} catch(org.omg.CosNaming.NamingContextPackage.InvalidName ex) { // resolve
			System.err.println("Failed to "+action+". NamingContext::InvalidName");
			System.exit(1);
		}
		catch(NotFound ex) { // resolve
			System.err.println("Failed to "+action+". NamingContext::NotFound");
			System.exit(1);
		}
		catch(CannotProceed ex) { // resolve
			System.err.println("Failed to "+action+". NamingContext::CannotProceed");
			System.exit(1);
		}
		catch(TRANSIENT ex) { // _narrow()
			System.err.println("Failed to "+action+". TRANSIENT");
			System.exit(1);
		}
		catch(OBJECT_NOT_EXIST ex) { // _narrow()
			System.err.println("Failed to "+action+". OBJECT_NOT_EXIST");
			System.exit(1);
		}
		catch (SystemException ex) {
			System.err.println("System exception, unable to "+action);
			System.exit(1);
		}
		catch (UserException ex) {
			System.err.println("CORBA exception, unable to "+action);
			System.exit(1);
		}

		return null;
	}

	/** The Java synchronization methods we use must be called from within the
	 *  object to be synchronized. Therefore we must do the work from an
	 * instance method, rather than from main().
	 */
	private void connect()
	{
		ProxyPushSupplier proxy_supplier = getProxyPushSupplier(consumer_admin);
		setDisconnectFlag(false);

		connectPushConsumer(proxy_supplier, consumer);
		// Wait for indication to disconnect before re-connecting.
		waitDisconnect();

		disconnectPushConsumer(proxy_supplier);
	} // end connect()

	static String[] stripOrbArgs(String[] args)
	{
		int len =0;
		for(int i=0; i<args.length; ++i)
		{
			if(args[i].startsWith("-ORB"))
				++i; // Skip this arg AND the next one.
			else
				args[len++]=args[i]; // keep this arg.
		}
		String[] result =new String[len];
		System.arraycopy(args,0,result,0,len);
		return result;
	}


	static void
	usage()
	{
		System.err.println(
				"\nCreate a PushConsumer to receive events from a channel.\n"
				+"syntax: java PushCons OPTIONS [CHANNEL_URI]\n"
				+"\n"
				+"CHANNEL_URI: The event channel may be specified as a URI.\n"
				+" This may be an IOR, or a corbaloc::: or corbaname::: URI.\n"
				+"\n"
				+"OPTIONS:                                         DEFAULT:\n"
				+" -d NUM   disconnect after receiving NUM events   [0 - never disconnect]\n"
				+" -s SECS  sleep SECS seconds after disconnecting  [0]\n"
				+" -n NAME  channel name (if URI is not specified)  [\"EventChannel\"]\n"
				+" -h       display this help text\n");
	}
} // end class PushCons
