package mockifer.example;

import static android.support.test.espresso.Espresso.onView;
import static android.support.test.espresso.Espresso.pressBack;
import static android.support.test.espresso.action.ViewActions.click;
import static android.support.test.espresso.assertion.ViewAssertions.matches;
import static android.support.test.espresso.contrib.RecyclerViewActions.actionOnItemAtPosition;
import static android.support.test.espresso.matcher.RootMatchers.isDialog;
import static android.support.test.espresso.matcher.ViewMatchers.isDisplayed;
import static android.support.test.espresso.matcher.ViewMatchers.withId;
import static android.support.test.espresso.matcher.ViewMatchers.withText;
import static mockifer.example.RecyclerViewCountAssertion.withItemCount;
import static org.hamcrest.CoreMatchers.allOf;

import android.support.test.espresso.action.ViewActions;
import android.support.test.rule.ActivityTestRule;
import android.support.test.runner.AndroidJUnit4;

import mockifer.Mockifer;
import mockifer.example.android.R;
import mockifer.example.android.main.MainActivity;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4.class)
public class ExampleInstrumentedTest {
    @Rule public ActivityTestRule<MainActivity> activityRule = new ActivityTestRule<>(MainActivity.class, true, false);

    @Before
    public void setUp() {
        Mockifer.reset();
    }

    private void launchActivity() {
        activityRule.launchActivity(null);
    }

    @Test
    public void viewMockCats() {
        // Before launching the test, we will preconfigure some mocks by pushing them on Mockifer.
        // There are also methods to push a mock to be run more than once, or pushing a collection
        // of mocks to save time in setup (they will all default to running once each).
        Mockifer.pushMock("mocks.cats.getfakecats");

        launchActivity();

        // Select the first cat and verify it was the mock that we pushed.
        onView(allOf(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition(0, click()));
        onView(withId(R.id.catName)).check(matches(withText("Mocky cat 1")));

        // Move back again, and this time the mock would have been consumed so the
        // regular data would load.
        pressBack();

        // Select the first cat and verify it was the default cat that isn't mocked.
        onView(allOf(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition(0, click()));
        onView(withId(R.id.catName)).check(matches(withText("Smish")));
    }

    @Test
    public void editCat() {
        launchActivity();

        // Select cat
        onView(allOf(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition(0, click()));

        // Update cat
        onView(withId(R.id.catName)).check(matches(withText("Smish")));
        onView(withId(R.id.catName)).perform(ViewActions.replaceText("Updated kitty"));
        onView(withId(R.id.saveButton)).perform(click());

        // Reselect same cat
        onView(allOf(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition(0, click()));
        onView(withId(R.id.catName)).check(matches(withText("Updated kitty")));
    }

    @Test
    public void deleteCat() {
        launchActivity();

        // Check that we start with 4 cats
        onView(withId(R.id.recyclerView)).check(withItemCount(4));

        // Select first cat
        onView(allOf(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition(0, click()));

        // Delete cat
        onView(withId(R.id.catName)).check(matches(withText("Smish")));
        onView(withId(R.id.deleteButton)).perform(click());

        // Check that we now have 3 cats
        onView(withId(R.id.recyclerView)).check(withItemCount(3));
    }

    @Test
    public void simulateInternal500Error() {
        Mockifer.pushMock("mocks.cats.getcats.error");
        launchActivity();

        onView(withText("Network Error"))
                .inRoot(isDialog())
                .check(matches(isDisplayed()));

        onView(withText("OK"))
                .inRoot(isDialog())
                .check(matches(isDisplayed()))
                .perform(click());
    }
}
