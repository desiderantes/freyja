// project created on 3/31/2005 at 1:20 PM
using System;
using Gtk;

enum MLispObjectGtkWidgetType
{
	MGTK_WIDGET = 64,
	MGTK_WINDOW,
	MGTK_BOX,
	MGTK_MENU,
	MGTK_MENUITEM,
	MGTK_BUTTON
}


public class MLispObjectGtkWidget : MLispObject {

	public MLispObjectGtkWidget(Gtk.Widget w) : base()
	{
		type = (uint)MLispObjectGtkWidgetType.MGTK_WIDGET;
		data = w;
		setTypeName("Gtk.Widget");
	}
}


class MainClass {

	public static MLispObject undef(ref MLispObjectList args)
	{
		Console.WriteLine("This method was called, but not implemented.");
		return new MLispObject();
	}


	public static MLispObject mgtk_handlebox(ref MLispObjectList args)
	{
		MLispObject pos = args.peek();
		MLispObject result = new MLispObject();

		HandleBox hbox = new HandleBox();

		if (pos.isNumeric())
		{
			switch ((int)pos.data)
			{
			case 0:
				hbox.HandlePosition = PositionType.Top;
				break;
			case 1:
				hbox.HandlePosition = PositionType.Left;
				break;
			case 2:
				hbox.HandlePosition = PositionType.Right;
				break;
			case 3:
				hbox.HandlePosition = PositionType.Bottom;
				break;
			}
			
			args.pop();
		}
	
		result = new MLispObjectGtkWidget(hbox);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_BOX;
		result.setTypeName("Gtk.HBox");
				
		return result;
	}


	// (button "_New" eMode eNewFile)
	public static MLispObject mgtk_button(ref MLispObjectList args)
	{
		MLispObject label = args.pop();
		MLispObject mode = args.pop();  // really unused
		MLispObject e = args.pop();
		MLispObject result = new MLispObject();
		Button button;


		if ((string)label.data == "_New")
		{
			button = new Button(Gtk.Stock.New);
		}
		else if ((string)label.data == "_Open")
		{
			button = new Button(Gtk.Stock.Open);
		}
		else if ((string)label.data == "_Save")
		{
			button = new Button(Gtk.Stock.Save);
		}
		else if ((string)label.data == "_Quit")
		{
			button = new Button(Gtk.Stock.Quit);
		}
		else
		{
			button = new Button((string)label.data);
		}
		
		// FIXME events not hooked up
		
		// FIXME: Temp hardcoded event hack
		button.Clicked += new EventHandler(clickedButtonCallback);
		
		result = new MLispObjectGtkWidget(button);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_BUTTON;
		result.setTypeName("Gtk.Button");
				
		return result;
	}
	
	
	static void clickedButtonCallback(object obj, EventArgs args)
	{
		Application.Quit();
	}
				

	public static MLispObject mgtk_menuitem(ref MLispObjectList args)
	{
		MLispObject name = args.pop();
		MLispObject accel = args.pop();
		MLispObject e;
		MLispObject result = new MLispObject();

		if (accel.type == (uint)MLispObjectType.CSTRING)
		{
			e = args.pop();
		}
		else
		{
			accel = new MLispObject();
			e = accel;
		}

		MLispObject icon = args.pop();

		MenuItem item = new MenuItem((string)name.data);
			
		result = new MLispObjectGtkWidget(item);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_MENUITEM;
		result.setTypeName("Gtk.MenuItem");
				
		return result;
	}


	public static MLispObject mgtk_menubar(ref MLispObjectList args)
	{
		MLispObject result = new MLispObject();
		MenuBar menubar = new MenuBar();


		result = new MLispObjectGtkWidget(menubar);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_MENU;
		result.setTypeName("Gtk.Menubar");
		
		MLispObject obj;
		while ((obj = args.pop()) != null && !obj.isNil())
		{
			if (obj.type == (uint)MLispObjectGtkWidgetType.MGTK_MENU)
			{
				Console.WriteLine("\t\t menubar.Append({0})", obj.type);	
				menubar.Append((Widget)obj.data);
			}
		}
		
		menubar.ShowAll();
		
		return result;
	}
	

	public static MLispObject mgtk_submenu(ref MLispObjectList args)
	{
		MLispObject name = args.pop();
		MLispObject result = new MLispObject();
		Menu menu = new Menu();
		menu.Title = (string)name.data;
		
		MLispObject obj;
		while ((obj = args.pop()) != null && !obj.isNil())
		{
			if (obj.type == (uint)MLispObjectGtkWidgetType.MGTK_MENUITEM ||
				obj.type == (uint)MLispObjectGtkWidgetType.MGTK_MENU)
			{
				Console.WriteLine("\t\t menu.Append({0})", obj.type);	
				menu.Append((Widget)obj.data);
			}
		}
		
		menu.ShowAll();
			
		result = new MLispObjectGtkWidget(menu);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_MENU;
		result.setTypeName("Gtk.Menu");
		
		return result;
	}

	public static MLispObject mgtk_window(ref MLispObjectList args)
	{
		MLispObject title = args.pop();
		MLispObject icon = args.pop();
		MLispObject result = new MLispObject();

		if (title == null || icon == null)
		{
			Console.WriteLine("ASSERT FAILED: mgtk_window(STRING, STRING)");
			return result;
		}
		
		Window window = new Window((string)title.data);
		window.SetDefaultSize(256, 256);				
		result = new MLispObjectGtkWidget(window);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_WINDOW;
		result.setTypeName("Gtk.Window");	

		Console.WriteLine("\t\tWINDOW");
	
		MLispObject obj;
		while ((obj = args.pop()) != null && !obj.isNil())
		{
			if (obj.type == (uint)MLispObjectGtkWidgetType.MGTK_BOX)
			{
				Console.WriteLine("\t\t window.Add({0})", obj.type);	
				window.Add((Widget)obj.data);
			}
		}
		
		window.ShowAll();
		
		return result;
	}
	

	public static MLispObject mgtk_hbox(ref MLispObjectList args)
	{
		MLispObject homogenous = args.pop();
		MLispObject spacing = args.pop();
		MLispObject expand = args.pop();
		MLispObject fill = args.pop();
		MLispObject padding = args.pop();
		MLispObject result = new MLispObject();
		HBox hbox;


		if (!homogenous.isNil() && !spacing.isNil())
		{
			hbox = new HBox(((int)homogenous.data == 1), (int)spacing.data);
		}
		else
		{
			hbox = new HBox();
		}
		
		result = new MLispObjectGtkWidget(hbox);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_BOX;
		result.setTypeName("Gtk.Hbox");

		Console.WriteLine("\t\tHBOX");
		
		MLispObject obj;
		while ((obj = args.pop()) != null && !obj.isNil())
		{
			if (obj.type >= 64)// (uint)MLispObjectGtkWidgetType.MGTK_BOX)
			{
				Console.WriteLine("\t\t hbox.PackStart({0})", obj.type);
					
				if (!expand.isNil() && !fill.isNil() && !padding.isNil())
				{
					hbox.PackStart((Widget)obj.data, 
									(int)expand.data == 1, (int)fill.data == 1,
									(uint)(int)padding.data);
				}
				else
				{
					hbox.PackStart((Widget)obj.data);
				}
			}
		}

		hbox.ShowAll();

		return result;
	}
	
	
	public static MLispObject mgtk_vbox(ref MLispObjectList args)
	{
		MLispObject homogenous = args.pop();
		MLispObject spacing = args.pop();
		MLispObject expand = args.pop();
		MLispObject fill = args.pop();
		MLispObject padding = args.pop();
		MLispObject result = new MLispObject();
		VBox vbox;


		if (!homogenous.isNil() && !spacing.isNil())
		{
			vbox = new VBox(((int)homogenous.data == 1), (int)spacing.data);
		}
		else
		{
			vbox = new VBox();
		}
		
		result = new MLispObjectGtkWidget(vbox);
		result.type = (uint)MLispObjectGtkWidgetType.MGTK_BOX;
		result.setTypeName("Gtk.VBox");
				
		Console.WriteLine("\t\tVBOX");
		
		MLispObject obj;
		while ((obj = args.pop()) != null && !obj.isNil())
		{
			if (obj.type >= 64)// (uint)MLispObjectGtkWidgetType.MGTK_BOX)
			{
				Console.WriteLine("\t\t vbox.PackStart({0})", obj.type);
				
				if (!expand.isNil() && !fill.isNil() && !padding.isNil())
				{
					vbox.PackStart((Widget)obj.data, 
									(int)expand.data == 1, (int)fill.data == 1,
									(uint)(int)padding.data);
				}
				else
				{	
					vbox.PackStart((Widget)obj.data);
				}
			}
		}

		vbox.ShowAll();

		return result;
	}


	public static void Main(string[] args)
	{
		Mlisp resource = new Mlisp();
		
		/* Register prototype MLispGtk# methods */
		resource.registerLispFunction("window", new MLispFunction(mgtk_window));
		resource.registerLispFunction("vbox", new MLispFunction(mgtk_vbox));
		resource.registerLispFunction("handlebox", new MLispFunction(mgtk_vbox/*handlebox*/)); // FIXME
		resource.registerLispFunction("menubar", new MLispFunction(mgtk_menubar));
		resource.registerLispFunction("menu_item", new MLispFunction(mgtk_menuitem));
		resource.registerLispFunction("submenu", new MLispFunction(mgtk_submenu));
		resource.registerLispFunction("button", new MLispFunction(mgtk_button));
		resource.registerLispFunction("hbox", new MLispFunction(mgtk_hbox));

		resource.registerLispFunction("icon", new MLispFunction(undef));
		resource.registerLispFunction("gl_widget",new MLispFunction(undef));
		resource.registerLispFunction("notebook", new MLispFunction(undef));
		resource.registerLispFunction("dialog", new MLispFunction(undef));
		resource.registerLispFunction("tab", new MLispFunction(undef));
		resource.registerLispFunction("hsep", new MLispFunction(undef));
		resource.registerLispFunction("vsep", new MLispFunction(undef));
		resource.registerLispFunction("textbox", new MLispFunction(undef));
		resource.registerLispFunction("label", new MLispFunction(undef));
		resource.registerLispFunction("label2", new MLispFunction(undef));
		resource.registerLispFunction("colorbutton", new MLispFunction(undef));
		resource.registerLispFunction("togglebutton", new MLispFunction(undef));
		resource.registerLispFunction("spinbutton", new MLispFunction(undef));
		resource.registerLispFunction("spinbutton2", new MLispFunction(undef));
		resource.registerLispFunction("menu_seperator", new MLispFunction(undef));
		resource.registerLispFunction("optionmenu", new MLispFunction(undef));
		resource.registerLispFunction("popup_menu", new MLispFunction(undef));
		resource.registerLispFunction("animation_tab_hack", new MLispFunction(undef));
		resource.registerLispFunction("toolbar", new MLispFunction(undef));
		resource.registerLispFunction("toolbar_box", new MLispFunction(undef));
		resource.registerLispFunction("toolbar_togglebutton", new MLispFunction(undef));
		resource.registerLispFunction("toolbar_button", new MLispFunction(undef));
		resource.registerLispFunction("hslider", new MLispFunction(undef));
		resource.registerLispFunction("dialog", new MLispFunction(undef));
		resource.registerLispFunction("fileselection_hack", new MLispFunction(undef));

		resource.registerSymbolValue("IconSize_Menu", 1);
		resource.registerSymbolValue("IconSize_ToolbarSmall", 2);
		resource.registerSymbolValue("IconSize_Toolbar", 3);
		resource.registerSymbolValue("IconSize_Button", 4);
		resource.registerSymbolValue("IconSize_Dialog", 5);

		resource.setDebugLevel(3); // 0, 3, 5
		
		/* Init Gtk# */
		Application.Init();
		
		resource.evalFile("../../test.mlisp");
		//resource.evalFile("../../freyja.mlisp");
		resource.dumpSymbols();

		/* Run Gtk# */
		Application.Run();
		
		//Application.Quit();
	}
}