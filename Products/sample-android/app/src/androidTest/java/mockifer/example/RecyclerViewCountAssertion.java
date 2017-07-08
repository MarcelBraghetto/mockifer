package mockifer.example;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

import android.support.test.espresso.NoMatchingViewException;
import android.support.test.espresso.ViewAssertion;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import org.hamcrest.Matcher;

class RecyclerViewCountAssertion implements ViewAssertion {
    private final Matcher<Integer> matcher;

    @Override public void check(View view, NoMatchingViewException noViewFoundException) {
        if (noViewFoundException != null) {
            throw noViewFoundException;
        }

        RecyclerView recyclerView = (RecyclerView) view;
        RecyclerView.Adapter adapter = recyclerView.getAdapter();
        assertThat(adapter.getItemCount(), matcher);
    }

    static RecyclerViewCountAssertion withItemCount(int expectedCount) {
        return withItemCount(is(expectedCount));
    }

    static RecyclerViewCountAssertion withItemCount(Matcher<Integer> matcher) {
        return new RecyclerViewCountAssertion(matcher);
    }

    private RecyclerViewCountAssertion(Matcher<Integer> matcher) {
        this.matcher = matcher;
    }
}

