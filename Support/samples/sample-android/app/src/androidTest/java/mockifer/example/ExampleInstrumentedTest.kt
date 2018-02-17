package mockifer.example

import android.support.test.espresso.Espresso.onView
import android.support.test.espresso.Espresso.pressBack
import android.support.test.espresso.action.ViewActions
import android.support.test.espresso.action.ViewActions.click
import android.support.test.espresso.assertion.ViewAssertions.matches
import android.support.test.espresso.contrib.RecyclerViewActions.actionOnItemAtPosition
import android.support.test.espresso.matcher.RootMatchers.isDialog
import android.support.test.espresso.matcher.ViewMatchers.*
import android.support.test.runner.AndroidJUnit4
import android.support.v7.widget.RecyclerView
import android.view.View
import mockifer.Mockifer
import mockifer.example.RecyclerViewCountAssertion.Companion.withItemCount
import mockifer.example.android.R
import mockifer.example.android.main.MainActivity
import org.hamcrest.CoreMatchers.allOf
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @JvmField @Rule var testRule: MockiferTestRule<*> = MockiferTestRule(MainActivity::class.java)

    @Before fun setup() {
        Mockifer.reset()
        Mockifer.clearActiveMocks()
    }

    @Test fun viewMockCats() {
        // Before beginning the test, we will preconfigure some mocks by pushing them on Mockifer.
        // There are also methods to push a mock to be run more than once, or pushing a collection
        // of mocks to save time in setup (they will all default to running once each).
        Mockifer.pushMock("mocks.cats.getfakecats")

        launchActivity()

        // Select the first cat and verify it was the mock that we pushed.
        onView(allOf<View>(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition<RecyclerView.ViewHolder>(0, click()))
        onView(withId(R.id.catNameEditText)).check(matches(withText("Mocky cat 1")))

        // Move back again, and this time the mock would have been consumed so the
        // regular data would load.
        pressBack()

        // Select the first cat and verify it was the default cat that isn't mocked.
        onView(allOf<View>(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition<RecyclerView.ViewHolder>(0, click()))
        onView(withId(R.id.catNameEditText)).check(matches(withText("Smish")))
    }

    @Test fun editCat() {
        launchActivity()

        // Select cat
        onView(allOf<View>(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition<RecyclerView.ViewHolder>(0, click()))

        // Update cat
        onView(withId(R.id.catNameEditText)).check(matches(withText("Smish")))
        onView(withId(R.id.catNameEditText)).perform(ViewActions.replaceText("Updated kitty"))
        onView(withId(R.id.saveButton)).perform(click())

        // Reselect same cat
        onView(allOf<View>(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition<RecyclerView.ViewHolder>(0, click()))
        onView(withId(R.id.catNameEditText)).check(matches(withText("Updated kitty")))
    }

    @Test fun deleteCat() {
        launchActivity()

        // Check that we start with 4 cats
        onView(withId(R.id.recyclerView)).check(withItemCount(4))

        // Select first cat
        onView(allOf<View>(withId(R.id.recyclerView), isDisplayed())).perform(actionOnItemAtPosition<RecyclerView.ViewHolder>(0, click()))

        // Delete cat
        onView(withId(R.id.catNameEditText)).check(matches(withText("Smish")))
        onView(withId(R.id.deleteButton)).perform(click())

        // Check that we now have 3 cats
        onView(withId(R.id.recyclerView)).check(withItemCount(3))
    }

    @Test fun simulateInternal500Error() {
        Mockifer.pushMock("mocks.cats.getcats.error")
        launchActivity()

        onView(withText("Network Error"))
                .inRoot(isDialog())
                .check(matches(isDisplayed()))

        onView(withText("OK"))
                .inRoot(isDialog())
                .check(matches(isDisplayed()))
                .perform(click())
    }

    private fun launchActivity() {
        testRule.launchActivity(null)
    }
}
