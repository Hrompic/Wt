
/*
 * Copyright (C) 2008 Emweb bv, Herent, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WMessageBox.h>

/*
 * A simple hello world application class which demonstrates how to react
 * to events, read input, and give feed-back.
 */
class HelloApplication : public Wt::WApplication
{
public:
  HelloApplication(const Wt::WEnvironment& env);

private:
  Wt::WLineEdit *nameEdit_;
  Wt::WText     *greeting_;

  void greet();
};

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
*/
HelloApplication::HelloApplication(const Wt::WEnvironment& env)
  : WApplication(env)
{
  setTitle("Hello world");                            // application title

  root()->addWidget(std::make_unique<Wt::WText>("Your name, please ? ")); // show some text

  nameEdit_ = root()->addWidget(std::make_unique<Wt::WLineEdit>()); // allow text input
  nameEdit_->setText("p1*ar");
  //  nameEdit_->setFocus();                              // give focus

  auto button = root()->addWidget(std::make_unique<Wt::WPushButton>("Greet me."));
  auto mbutton = root()->addWidget(std::make_unique<Wt::WPushButton>("Run!!!"));
  auto mtext = root()->addWidget(std::make_unique<Wt::WText>());
  mbutton->clicked().connect([=]
          {
            auto mbox =  root()->addChild(std::make_unique<Wt::WMessageBox>( 
              "Status",
	          "<p>Ready to launch the rocket...</p>"
	          "<p>Launch the rocket immediately?</p>",
              Wt::Icon::Information,
              Wt::StandardButton::Yes | Wt::StandardButton::No));
            mbox->setModal(false);
            mbox->buttonClicked().connect([=]
                    {
                        if(mbox->buttonResult()==Wt::StandardButton::Yes)
                            mtext->setText("<H1>The rocket is launched!!!!</H1>");
                        else
                            mtext->setText("Launching the rocket has canceled!");
                        root()->removeChild(mbox);
                    });
            mbox->show();
          });
  // create a button
  button->setFocus();
  button->setMargin(5, Wt::Side::Left);                   // add 5 pixels margin

  root()->addWidget(std::make_unique<Wt::WBreak>());    // insert a line break
  greeting_ = root()->addWidget(std::make_unique<Wt::WText>()); // empty text

  /*
   * Connect signals with slots
   *
   * - simple Wt-way: specify object and method
   */
  button->clicked().connect(this, &HelloApplication::greet);

  /*
   * - using an arbitrary function object, e.g. useful to bind
   *   values with std::bind() to the resulting method call
   */
  nameEdit_->enterPressed().connect(std::bind(&HelloApplication::greet, this));

  /*
   * - using a lambda:
   */
  button->clicked().connect([=]() { 
      std::cerr <<"\033[1;31mHello there, " <<nameEdit_->text() <<"\033[0m" << std::endl;
  });
}

void HelloApplication::greet()
{
  /*
   * Update the text, using text input into the nameEdit_ field.
   */
  greeting_->setText("Hello there, " + nameEdit_->text());
}

int main(int argc, char **argv)
{
  /*
   * Your main method may set up some shared resources, but should then
   * start the server application (FastCGI or httpd) that starts listening
   * for requests, and handles all of the application life cycles.
   *
   * The last argument to WRun specifies the function that will instantiate
   * new application objects. That function is executed when a new user surfs
   * to the Wt application, and after the library has negotiated browser
   * support. The function should return a newly instantiated application
   * object.
   */
  return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
    /*
     * You could read information from the environment to decide whether
     * the user has permission to start a new application
     */
    return std::make_unique<HelloApplication>(env);
  });
}
